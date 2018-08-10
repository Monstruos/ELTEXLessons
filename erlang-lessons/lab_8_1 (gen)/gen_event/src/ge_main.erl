-module (ge_main).

-behaviour (gen_event).

-export ([init/1, handle_call/2, handle_event/2, handle_info/2, terminate/2,
    code_change/3]).

-export ([start_link/0, stop/0, make_event/1, make_request/0]).

% Client call functions
start_link() ->
    Result = gen_event:start_link({global, ?MODULE}),
    gen_event:add_handler({global, ?MODULE}, ?MODULE, []),
    Result.

stop() ->
    gen_event:stop({global, ?MODULE}).

make_event(Event) ->
    gen_event:notify({global, ?MODULE}, Event).

make_request() ->
    gen_event:call({global, ?MODULE}, ?MODULE, request).

% Call back functions
init([]) ->
    {ok, {0, []}}.

handle_call(_Request, {Count, Events}) ->
    io:format("~p events happend!~p~nNow is 0!~n", [Count, Events]),
    {ok, Count, {0, []}}.

handle_event(Event, {Count, Events}) ->
    io:format("~p happend!~n", [Event]),
    {ok, {Count + 1, Events ++ [Event]}}.

handle_info(Info, State) ->
    io:format("~p: ~p~n", [Info, State]),
    {ok, Info, State}.

terminate(_Reason, _State) ->
    ok.

code_change(_OldVer, State, _Extra) ->
    {ok, State}.
