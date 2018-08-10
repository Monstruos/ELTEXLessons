-module(lab_7_app).

-behaviour(application).

%% Application callbacks
-export([start/2, stop/1]).

%% ===================================================================
%% Application callbacks
%% ===================================================================

start(_StartType, _StartArgs) ->
    case application:get_env(lab_7, access_token) of
        undefined ->
            io:format("Define access_token, please~n"),
            ok;
        _ ->
            inets:start(),
            lab_7_sup:start_link()
    end.

stop(_State) ->
    ok.
