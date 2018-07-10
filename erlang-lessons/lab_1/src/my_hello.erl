-module(my_hello).

-export([test/0]).

test() ->
    io:format("Hello, world~n"),
    B = math:sqrt(8),
    io:format("~p~n", [B]).
