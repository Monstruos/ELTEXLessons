-module(lab_6_2_client_tcp).

-export ([echo_client_tcp/3]).


echo_client_tcp(Address, Port, "stop().") ->
    {ok, Socket} = gen_tcp:connect(Address, Port, [{active, false}]),
    gen_tcp:send(Socket, "stop()."),
    gen_tcp:shutdown(Socket, read_write),
    gen_tcp:close(Socket);
echo_client_tcp(Address, Port, Packet) ->
    {ok, Socket} = gen_tcp:connect(Address, Port, [{active, false}]),
    gen_tcp:send(Socket, Packet),
    {ok, RecvMes} = gen_tcp:recv(Socket, 0),
    gen_tcp:close(Socket),
    RecvMes.
