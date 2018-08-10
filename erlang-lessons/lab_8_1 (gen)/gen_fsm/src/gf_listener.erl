-module (gf_listener).

-export([create_listener/0, wait_call/0]).

create_listener() ->
    spawn(?MODULE, wait_call, []).

% 'Listener'
wait_call() ->
    receive
        ring ->
            io:format("[~p]:Incoming call!~n", [self()]),
            gf_main:up_phone();
        _ ->
            io:format("[~p]:Error~n", [self()])
    end.
