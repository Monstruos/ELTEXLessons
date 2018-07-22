-module(lab_4_1).

-export([fact/1, fact_tail/2, time_fact/1, time_fact_tail/1]).

% run:lab_4_1:time_fact(1000).
%     lab_4_1:time_fact_tail(1000).

fact(0) ->
    1;
fact(N) when N > 0 ->
    N * fact(N - 1).

fact_tail(0, Res) ->
    Res;
fact_tail(N, Res) when N > 0 ->
    fact_tail(N - 1, N * Res).

time_fact(N) ->
    timer:tc(lab_4_1, fact, [N]).

time_fact_tail(N) ->
    timer:tc(lab_4_1, fact_tail, [N, 1]).
