-module(lab_4_2).

-export([qsort/1, qsort_tail/1, time_qsort/1, time_qsort_tail/1]).

% run:lab_4_2:time_qsort([rand:uniform(10)||_ <- lists:seq(1, 1000)]).
%     lab_4_2:time_qsort_tail([rand:uniform(10)||_ <- lists:seq(1, 1000)]).

qsort([]) ->
    [];
qsort([Head|Tail]) ->
    qsort([X || X <- Tail, X =< Head]) ++
    [Head] ++ qsort([X || X <- Tail, X > Head]).

qsort_tail([]) -> [];
qsort_tail([X]) -> [X];
qsort_tail([Split|Tail]) ->
    {Small, Great} = qsort_tail(Split, Tail),
    SortSmall = qsort_tail(Small),
    SortGreat = qsort_tail(Great),
    SortSmall ++ [Split] ++ SortGreat.

qsort_tail(Split, Tail) ->
    qsort_tail(Split, [], [], Tail).

qsort_tail(_Head, Small, Great, []) ->
    {Small, Great};
qsort_tail(Split, Small, Great, [Head|Tail]) when Head =< Split ->
    qsort_tail(Split, [Head|Small], Great, Tail);
qsort_tail(Split, Small, Great, [Head|Tail]) when Head > Split ->
    qsort_tail(Split, Small, [Head|Great], Tail).


time_qsort(N) ->
    timer:tc(lab_4_2, qsort, [N]).

time_qsort_tail(N) ->
    timer:tc(lab_4_2, qsort_tail, [N]).
