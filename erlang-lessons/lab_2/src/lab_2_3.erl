-module(lab_2_3).

-export([th_elem/1]).

% Change every third element to -1
% example: lab_2_3:th_elem([2, 3, 4, 6, 8, 2, 3, 21, 4]).
th_elem([H1, H2, _|T]) ->
    [H1, H2, -1| th_elem(T)];
th_elem(List) ->
    List.
