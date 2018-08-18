-module (uri_encoder).

-export ([encode/1]).

% Source: https://gist.github.com/renatoalbano/3796470
encode([C | Cs]) when C >= $a, C =< $z ->
    [C | encode(Cs)];
encode([C | Cs]) when C >= $A, C =< $Z ->
    [C | encode(Cs)];
encode([C | Cs]) when C >= $0, C =< $9 ->
    [C | encode(Cs)];

encode([C | Cs]) when C == 16#20 ->
    [$+ | encode(Cs)];

encode([C = $- | Cs]) ->
    [C | encode(Cs)];
encode([C = $_ | Cs]) ->
    [C | encode(Cs)];
encode([C = 46 | Cs]) ->
    [C | encode(Cs)];
encode([C = $! | Cs]) ->
    [C | encode(Cs)];
encode([C = $~ | Cs]) ->
    [C | encode(Cs)];
encode([C = $* | Cs]) ->
    [C | encode(Cs)];
encode([C = 39 | Cs]) ->
    [C | encode(Cs)];
encode([C = $( | Cs]) ->
    [C | encode(Cs)];
encode([C = $) | Cs]) ->
    [C | encode(Cs)];

encode([C | Cs]) when C =< 16#7f ->
    escape_byte(C)
        ++ encode(Cs);

encode([C | Cs]) when (C >= 16#7f) and (C =< 16#07FF) ->
    escape_byte((C bsr 6) + 16#c0)
        ++ escape_byte(C band 16#3f + 16#80)
        ++ encode(Cs);

encode([C | Cs]) when (C > 16#07FF) ->
    escape_byte((C bsr 12) + 16#e0)
        ++ escape_byte((16#3f band (C bsr 6)) + 16#80)
        ++ escape_byte(C band 16#3f + 16#80)
        ++ encode(Cs);

encode([C | Cs]) ->
    escape_byte(C) ++ encode(Cs);

encode([]) -> [].

hex_octet(N) when N =< 9 ->
    [$0 + N];
hex_octet(N) when N > 15 ->
    hex_octet(N bsr 4) ++ hex_octet(N band 15);
hex_octet(N) ->
    [N - 10 + $a].

escape_byte(C) ->
    H = hex_octet(C),
    normalize(H).

normalize(H) when length(H) == 1 ->
    "%0" ++ H;

normalize(H) ->
    "%" ++ H.
