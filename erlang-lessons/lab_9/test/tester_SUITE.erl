-module (tester_SUITE).

-include_lib("common_test/include/ct.hrl").
-compile(export_all).

% Run: ct_run -dir ./test ./src


all() ->
    [
        th_elem_tests,
        reverse_tests,
        get_infinity_tests
    ].

init_per_suite(Config) ->
    Config.

init_per_testcase(_, Config) ->
    Config.

end_per_testcase(_, Config) ->
    Config.

end_per_suite(Config) ->
    Config.

th_elem_tests(_Config) ->
    try
        tester:th_elem(23)
    catch
        error:function_clause ->
            ok
    end,
    [2, 3] = tester:th_elem([2, 3]),
    [2, 3, -1, 5] = tester:th_elem([2, 3, 4, 5]).

reverse_tests(_Config) ->
    [] = tester:reverse([]),
    [a, 2, 3] = tester:reverse([3, 2, a]),
    try
        tester:reverse(23)
    catch
        error:function_clause ->
            ok
    end.

get_infinity_tests(_Config) ->
    try
        tester:get_infinity(10)
    catch
        error:badarith ->
            ok
    end.
