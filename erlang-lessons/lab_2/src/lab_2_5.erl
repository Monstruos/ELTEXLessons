-module(lab_2_5).

-export([ackermann/2]).

% Ackermann function
% example: lab_2_5:ackermann(3, 3). if you wanna have a cup of coffee, you can
% run ackermann(4, 2).

ackermann(0, N) when N >= 0->
    N + 1;
ackermann(M, 0) when M > 0 ->
    ackermann(M - 1, 1);
ackermann(M, N) when M > 0, N > 0 ->
    ackermann(M - 1, ackermann(M, N - 1)).
