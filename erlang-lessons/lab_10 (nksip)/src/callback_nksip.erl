-module (callback_nksip).

-export ([sip_options/2, sip_invite/2]).

sip_invite(Req, _Call) ->
    {ok, Body} = nksip_request:body(Req),
    case nksip_sdp:is_sdp(Body) of
        true ->
            {ok, ReqId} = nksip_request:get_handle(Req),
            Fun = fun() ->
                nksip_request:reply(ringing, ReqId),
                timer:sleep(2000),
                nksip_request:reply({answer, Body}, ReqId)
            end,
            spawn(Fun),
            noreply;
        false ->
            {reply, {not_acceptable, <<"Invalid SDP">>}}
    end.

sip_options(Req, _Call) ->
    {ok, AppName} = nksip_request:srv_name(Req),
    {reply, {ok, [{add, "x-nk-id", AppName},
        contact, allow, accept, supported]}}.
