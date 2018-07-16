-module(lab_2_4).

-export([reverse/1]).

% Reverse the list
% example: lab_2_4:reverse([2, 3, 4, 6, 8, 2, 3, 21, 4]).

reverse([]) ->
    [];
reverse([H|T]) ->
    reverse(T) ++ [H].
