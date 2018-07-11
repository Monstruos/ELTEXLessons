-module (lab_2_2).

-export ([sumMoney/1]).

% For reading records to the shell: rr(module_name).
% UserList = [#bankuser{id=0, name="First", money=1},
%             #bankuser{id=1, name="Second", money=3},
%             #bankuser{id=2, name="Third", money=4}].
-record (bankuser, {id, name, money = 0}).

% sum of user's money
sumMoney(UserList) ->
    sumMoney(UserList, 0).

sumMoney([], N) ->
    N;
sumMoney([H|T], N) when is_record(H, bankuser) ->
    sumMoney(T, N + H#bankuser.money).
