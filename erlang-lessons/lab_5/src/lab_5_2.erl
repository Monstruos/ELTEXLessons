-module(lab_5_2).

-import(phonebook, [get_records_from_phone_book/1, search_by_name/2]).
-export([db_handler/1]).

% Database handler.
% note: usage with another pid:
%     Handler = spawn(lab_5_2, db_handler, ['ebin/buf.txt']).
%     Handler ! {self(), 'First'}.
%     flush().

db_handler(DbFile) ->
    DbList = get_records_from_phone_book(DbFile),
    receive
        {SendPid, AnyName} ->
            SendPid ! search_by_name(DbList, AnyName),
            db_handler(DbFile);
        stop_handle ->
            ok
    end.
