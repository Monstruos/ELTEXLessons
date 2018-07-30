-module (lab_6_2_server_tcp).

-export ([echo_server/2]).

echo_server(start, [Port]) ->
    {ok, Socket} = gen_tcp:listen(Port, [{ip, {127, 0, 0, 1}}, list,
        {active, false}]),
    echo_server(wait, [Socket]);


echo_server(wait, [LSocket]) ->
    case gen_tcp:accept(LSocket, 50000) of
        {ok, Socket} ->
            echo_server(recv, [Socket, LSocket]);
        {error, Reason} ->
            echo_server(alarm, [Reason, LSocket])
    end;

echo_server(recv, [Socket, LSocket]) ->
    case gen_tcp:recv(Socket, 0, 50000) of
        {ok, ClPacket} ->
            echo_server(handle, [Socket, ClPacket, LSocket]);
        {error, Reason} ->
            echo_server(alarm, [Reason, LSocket])
    end;

echo_server(handle, [Socket, ClPacket, LSocket]) ->
    case string:equal(ClPacket, "stop().") of
        false ->
            ServPack = ClPacket ++ ", client!",
            echo_server(send, [Socket, ServPack, LSocket]);
        true ->
            echo_server(close, [Socket]),
            echo_server(close, [LSocket])
    end;

echo_server(send, [Socket, ServPack, LSocket]) ->
    case gen_tcp:send(Socket, ServPack) of
        ok ->
            echo_server(close, [Socket]),
            echo_server(wait, [LSocket]);
        {error, Reason} ->
            echo_server(alarm, [Reason, LSocket])
    end;

echo_server(close, [Socket]) ->
    gen_tcp:shutdown(Socket, read_write),
    gen_tcp:close(Socket);

echo_server(alarm, [AlarmMsg, LSocket]) ->
    echo_server(close, [LSocket]),
    {error, AlarmMsg}.
