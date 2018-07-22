-module(lab_5_1).

-export([pow2/1]).

-define(POW2(P),
begin
    Fun = fun(A, F) ->
        if
            A == 0 ->
                1;
            A > 0 ->
                2 * F(A - 1, F)
        end
    end,
    Fun(P, Fun)
end
).

pow2(P) ->
    Res = ?POW2(P),
    Res.
