-module(st_wrapper).
-include_lib("chronica/include/chronica.hrl").
-chronica_tag(statem_log).

-behaviour(gen_statem).

%% API
-export([start_link/0]).

%% gen_statem callbacks
-export([
    init/1,
    format_status/2,
    q0/3,
    q1/3,
    q2/3,
    terminate/3,
    code_change/4,
    callback_mode/0
]).
-export([cipher/1, refresh/0]).

-define(SERVER, ?MODULE).

-record(state, {}).

%% ===================================================================
%% API
%% ===================================================================

start_link() ->
    gen_statem:start_link({local, ?SERVER}, ?MODULE, [], []).

%% ===================================================================
%% gen_statem callbacks
%% ===================================================================


init([]) ->
    {ok, q0, #state{}}.

callback_mode() ->
    state_functions.

format_status(_Opt, [_PDict, _StateName, _State]) ->
    Status = something_bad,
    Status.

terminate(_Reason, _StateName, _State) ->
    ok.

code_change(_OldVsn, StateName, State, _Extra) ->
    {ok, StateName, State}.

q0({call, From}, EventContent, State) ->
    case EventContent of
        0 ->
            {next_state, q2, 0, [{reply, From, 0}]};
        1 ->
            {next_state, q1, 0, [{reply, From, 0}]};
        _else ->
            handle_other({call, From}, EventContent, State)
    end.

q1({call, From}, EventContent, State) ->
    case EventContent of
        0 ->
            {next_state, q2, 1, [{reply, From, 1}]};
        1 ->
            {keep_state, 1, [{reply, From, 1}]};
        _else ->
            handle_other({call, From}, EventContent, State)
    end.

q2({call, From}, EventContent, State) ->
    case EventContent of
        1 ->
            {next_state, q1, 0, [{reply, From, 0}]};
        0 ->
            {keep_state, 0, [{reply, From, 0}]};
        _else ->
            handle_other({call, From}, EventContent, State)
    end.

handle_other({call, From}, refresh, State) ->
    {next_state, q0, 0, [{reply, From, ok}]};
handle_other(_, _, State) ->
    {keep_state, State, []}.

%% ===================================================================
%% User API
%% ===================================================================

cipher(Input) when erlang:is_integer(Input) ->
    gen_statem:call(?SERVER, Input);
cipher(List) when erlang:is_list(List) ->
    cipher(List, []).

cipher([Head|List], ResList) ->
    Res = gen_statem:call(?SERVER, Head),
    cipher(List, ResList ++ [Res]);
cipher([], ResList) ->
    ResList.


refresh() ->
    gen_statem:call(?SERVER, refresh).