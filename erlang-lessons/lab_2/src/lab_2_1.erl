-module(lab_2_1).

-export([pow/2, fact/1]).

%% Power
pow(_Base, 0) ->
    1;
pow(Base, Pow) when Pow >= 1 ->
    Base * pow(Base, Pow - 1);
pow(Base, Pow) when Pow =< -1 ->
    pow(Base, Pow + 1) / Base.


%% Factorial
fact(0) ->
    1;
fact(N) when N >= 1 ->
    N * fact(N - 1).
