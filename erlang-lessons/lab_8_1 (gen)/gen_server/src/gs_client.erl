-module (gs_client).

-export ([ask_date/0, ask_time/0]).

ask_date() ->
    gs_server:ask_date().

ask_time() ->
    gs_server:ask_time().
