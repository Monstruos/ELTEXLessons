-module (lab_6_2_server_udp).

-export ([echo_server/2]).

echo_server(start, [Port]) ->
    {ok, Socket} = gen_udp:open(Port, [{ip, {127, 0, 0, 1}},
        list, {active, true}]),
    echo_server(wait, [Socket]);


echo_server(wait, [Socket]) ->
    receive
        {udp, Socket, Addr, ClPort, ClPacket} ->
            echo_server(handle, [Socket, Addr, ClPort, ClPacket]);
        true ->
            echo_server(alarm, ["Receive error"])
    end;

echo_server(handle, [Socket, Addr, ClPort, ClPacket]) ->
    case string:equal(ClPacket, "stop().") of
        false ->
            ServPack = ClPacket ++ ", client!",
            echo_server(send, [Socket, Addr, ClPort, ServPack]);
        true ->
            echo_server(close, [Socket])
    end;

echo_server(send, [Socket, Addr, ClPort, ServPack]) ->
    case gen_udp:send(Socket, Addr, ClPort, ServPack) of
        ok ->
            echo_server(wait, [Socket]);
        {error, Reason} ->
            echo_server(alarm, [Reason])
    end;

echo_server(close, [Socket]) ->
    gen_udp:close(Socket);

echo_server(alarm, [AlarmMsg]) ->
    {error, AlarmMsg}.
