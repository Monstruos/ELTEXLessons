-module (nksip_wrap).

-export([start_link/0, start/0, stop/1, calling/0]).

start_link() ->
    {ok, spawn(?MODULE, calling, [])}.

calling() ->
    DlgId = start(),
    timer:sleep(5000),
    stop(DlgId).

start() ->
    nksip:start(client1, #{
        sip_from => "sip:0987@bowser.peach",
        callback => callback_nksip,
        sip_listen => "sip:all:5070, sip:all:5071",
        plugins => [nksip_uac_auto_auth]
    }),
    {ok, 200, _} = nksip_uac:register(client1, "sip:192.168.2.81",
        [{sip_pass, "1"}, contact]),
    {ok, 200, [{dialog, DlgId}]} = nksip_uac:invite(client1,
        "sip:5318008@bowser.peach",
        [
            {route, "<sip:192.168.2.81;lr>"},
            {body, nksip_sdp:new()},
            {sip_pass, "1"},
            auto_2xx_ack
        ]),
    DlgId.

stop(DlgId) ->
    {ok, 200, []} = nksip_uac:bye(DlgId, []),
    nksip:stop_all().
