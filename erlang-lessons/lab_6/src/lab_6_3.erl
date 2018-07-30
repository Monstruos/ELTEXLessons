-module (lab_6_3).

-export ([start_link/1]).
-export ([web_server/2]).

start_link(Port) ->
    Server = spawn(?MODULE, web_server, [start, [Port]]),
    {ok, Server}.

% run web-server on loopback interface
web_server(start, [Port]) ->
    {ok, Socket} = gen_tcp:listen(Port, [{ip, {127, 0, 0, 1}}, list,
        {active, false}]),
    web_server(wait, [Socket]);


web_server(wait, [LSocket]) ->
    case gen_tcp:accept(LSocket, 50000) of
        {ok, Socket} ->
            web_server(recv, [Socket, LSocket]);
        {error, Reason} ->
            web_server(alarm, [Reason, LSocket])
    end;

web_server(recv, [Socket, LSocket]) ->
    case gen_tcp:recv(Socket, 0, 50000) of
        {ok, ClPacket} ->
            web_server(handle, [Socket, ClPacket, LSocket]);
        {error, Reason} ->
            web_server(alarm, [Reason, LSocket])
    end;

web_server(handle, [Socket, ClPacket, LSocket]) ->
    [Request| _] = binary:split(list_to_binary(ClPacket), <<"\r\n">>,
        [global]),
    [_, FileBin| _] = binary:split(Request, <<" ">>, [global]),
    File = binary_to_list(FileBin),
    [_|FileName] = File,
    case string:equal(FileName, "stop()") of
        false ->
            {ok, Data} = file:read_file(FileName),
            web_server(send, [Socket, set_answer(binary_to_list(Data)),
                LSocket]);
        true ->
            web_server(close, [Socket]),
            web_server(close, [LSocket])
    end;

web_server(send, [Socket, ServAns, LSocket]) ->
    case gen_tcp:send(Socket, ServAns) of
        ok ->
            web_server(close, [Socket]),
            web_server(wait, [LSocket]);
        {error, Reason} ->
            web_server(alarm, [Reason, LSocket])
    end;

web_server(close, [Socket]) ->
    gen_tcp:shutdown(Socket, read_write),
    gen_tcp:close(Socket);

web_server(alarm, [AlarmMsg, LSocket]) ->
    web_server(close, [LSocket]),
    {error, AlarmMsg}.

% Add request-header to data from file for right-handle by browser
set_answer(Data) ->
    "HTTP/1.1 200 OK\r\nContent-Length:" ++
        integer_to_list(lists:flatlength(Data)) ++ "\r\n\r\n" ++
        Data.
