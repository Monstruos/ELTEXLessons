-module (tester_tests).

-compile(export_all).
-include_lib("eunit/include/eunit.hrl").


test1_th_elem_test() ->
    ?assertError(function_clause, tester:th_elem(1)).

test2_th_elem_test() ->
    ?assertEqual([2, 3], tester:th_elem([2, 3])).

test3_th_elem_test() ->
    ?assertEqual([2, 3, -1, 5], tester:th_elem([2, 3, 4, 5])).

reverse_test_() ->
    [
        ?_assert(tester:reverse([]) =:= []),
        ?_assert(tester:reverse([2, 3, a]) =:= [a, 3, 2]),
        ?_assertError(function_clause, tester:reverse(42))
    ].

get_infinity_test_() ->
    [
        ?_assertException(error, badarith, tester:get_infinity(42))
    ].
