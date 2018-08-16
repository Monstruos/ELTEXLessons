-module(tester).

-export ([th_elem/1, reverse/1, get_infinity/1]).

th_elem([H1, H2, _|T]) ->
    [H1, H2, -1| th_elem(T)];
th_elem(List) when is_list(List) ->
    List.

reverse([]) ->
    [];
reverse([H|T]) ->
    reverse(T) ++ [H].

get_infinity(Num) when is_number(Num) ->
    Num / 0.
