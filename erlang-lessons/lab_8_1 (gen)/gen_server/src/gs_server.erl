-module (gs_server).

-behaviour (gen_server).

-export ([init/1, handle_call/3, handle_cast/2, handle_info/2, terminate/2,
    code_change/3]).
-export ([start_link/0, stop/0, ask_date/0, ask_time/0]).

% Client call functions
start_link() ->
    gen_server:start_link({global, ?MODULE}, ?MODULE, [], []).

stop() ->
    gen_server:call({global, ?MODULE}, stop).

ask_date() ->
    gen_server:call({global, ?MODULE}, {get_date}).

ask_time() ->
    gen_server:call({global, ?MODULE}, {get_time}).

% Call back functions
init([]) ->
    process_flag(trap_exit, true),
    {ok, []}.

handle_call({get_date}, _From, State) ->
    {reply, gs_logic:handler(date), State};

handle_call({get_time}, _From, State) ->
    {reply, gs_logic:handler(time), State};

handle_call(_Req, _From, State) ->
    {reply, wrong_req, State}.

handle_cast(_Req, State) ->
    {noreply, State}.

handle_info(Info, State) ->
    {noreply, Info, State}.

terminate(_Reason, _State) ->
    ok.

code_change(_OldVer, State, _Extra) ->
    {ok, State}.
