-module (gs_logic).

-export ([handler/1]).

handler(Inp) ->
    case Inp of
        date ->
            {{Year, Month, Day}, _Time} =
                calendar:now_to_datetime(erlang:timestamp()),
            io:format("Date: ~pd ~pm ~py~n", [Day, Month, Year]),
            {Year, Month, Day};
        time ->
            {_Date, {H, M, S}} =
                calendar:now_to_datetime(erlang:timestamp()),
            io:format("Time: ~ph ~pm ~ps~n", [H, M, S]),
            {H, M, S};
        _ ->
            ok
    end.
