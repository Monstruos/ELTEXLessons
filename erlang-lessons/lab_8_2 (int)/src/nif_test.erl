-module (nif_test).
-on_load(on_load/0).
-export([fact/1]).

on_load() ->
    erlang:load_nif("priv/nif_test", 0).

fact(_) ->
    {error, no_load}.
