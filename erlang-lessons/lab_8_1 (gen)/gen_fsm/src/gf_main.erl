-module (gf_main).

-behaviour (gen_fsm).

-export ([init/1, wait/2, talk/2, call/2, handle_event/3, handle_info/3,
    handle_sync_event/4, terminate/3, code_change/4]).
-export ([start_link/0, call/1, up_phone/0, stop_talk/0, start_apocalypse/0]).

% Client call functions
start_link() ->
    gen_fsm:start_link({global, ?MODULE}, ?MODULE, [], []).

call(ToPid) ->
    gen_fsm:send_event({global, ?MODULE}, {self(), ToPid}).

up_phone() ->
    gen_fsm:send_event({global, ?MODULE}, {up_phone, self()}).

stop_talk() ->
    gen_fsm:send_event({global, ?MODULE}, down_phone).

start_apocalypse() ->
    gen_fsm:send_all_state_event({global, ?MODULE}, error).


% Call back functions
init([]) ->
    io:format("-> waiting~n"),
    {ok, wait, []}.

wait({FromPid, ToPid}, []) ->
    io:format("-> call from ~p to ~p~n", [FromPid, ToPid]),
    ToPid ! ring,
    {next_state, call, [FromPid, ToPid]}.

call({up_phone, ToPid}, [_FromPid, ToPid]) ->
    io:format("-> ~p is connected!~n", [ToPid]),
    {next_state, talk, []}.

talk(down_phone, _Args) ->
    {next_state, wait, []}.

handle_event(error, _StateName, _StateData) ->
    {stop, "apocalypse", []}.

handle_sync_event(error, _From, _StateName, _StateData) ->
    {stop, "apocalypse", []}.

handle_info(_Info, _StateName, _StateData) ->
    {stop, normal, []}.

terminate(Reason, StateName, StateData) ->
    {terminate, Reason, StateName, StateData}.

code_change(_OldVsn, _StateName, _StateData, _Extra) ->
    {ok, wait, []}.
