-module(lab_6_1_client).

-export([rpc_call_logic/2, spawn_start_logic/1, spawn_request/2]).

% Running logic on another node by rpc:call
rpc_call_logic(NodeName, Filename) ->
    % business_logic after sending turn into wait state node, that calling
    % this function has no access to node by "rpc:call", so call with timeout,
    % ignore error 'badrpc' and show sended message by flush
    rpc:call(NodeName, lab_6_1, business_logic,
        [open_file, {self(), Filename}], 100),
    c:flush().

% Running logic on another node by spawn. return Pid of another node's process.
spawn_start_logic(NodeName) ->
    spawn(NodeName, lab_6_1, business_logic, [start, {}]).


% Send request "open 'Filename'" to process spawned by spawn_start_logic/2 or
% stop it
spawn_request(NodePid, stop) ->
    NodePid ! stop;
spawn_request(NodePid, FileName) ->
    NodePid ! {self(), FileName}.
