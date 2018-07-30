-module(lab_6_sup).

-behaviour(supervisor).

%% API
-export([start_link/0]).

%% Supervisor callbacks
-export([init/1]).

%% Helper macro for declaring children of supervisor
-define(CHILD(I, Module, Type, Port),
    {I,
    {Module, start_link, [Port]},
    permanent,
    5000,
    Type,
    [Module]}).

%% ===================================================================
%% API functions
%% ===================================================================

start_link() ->
    supervisor:start_link({local, ?MODULE}, ?MODULE, []).

%% ===================================================================
%% Supervisor callbacks
%% ===================================================================

init([]) ->
    {ok, { {one_for_one, 5, 10}, [
        ?CHILD(lab_6_3_1, lab_6_3, worker, 80),
        ?CHILD(lab_6_3_2, lab_6_3, worker, 65000),
        ?CHILD(lab_6_3_3, lab_6_3, worker, 65001)]} }.
