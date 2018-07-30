-module(lab_6_2_client_udp).

-export ([echo_client_udp/4]).


echo_client_udp(SelfPort, Address, Port, Packet) ->
    {ok, Socket} = gen_udp:open(SelfPort, [{active, true}]),
    gen_udp:send(Socket, Address, Port, Packet),
    receive
        {udp, Socket, _Addr, _ClPort, ClPacket} ->
            io:format("~p~n", [ClPacket]);
        true ->
            ok
    end,
    gen_udp:close(Socket).
