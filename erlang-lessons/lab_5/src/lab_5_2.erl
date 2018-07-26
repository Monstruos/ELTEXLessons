-module(lab_5_2).

-import(phonebook, [get_records_from_phone_book/1, search_by_name/2]).
-export([db_handler/1]).

% Database handler.
% note: usage with another pid:
%     Handler = spawn(NodeName, lab_5_2, db_handler, ['ebin/buf.txt']).
%     Handler ! {self(), 'First'}.
%     flush().

db_handler(DbFile) ->
    DbList = get_records_from_phone_book(DbFile),
    db_hand_loop(DbList).

db_hand_loop(DbList) ->
    receive
        {SendPid, AnyName} ->
            SendPid ! search_by_name(DbList, AnyName),
            db_hand_loop(DbList);
        % This part is not working, still have no ideas why
        % {SendPid, HostName, AnyName} ->
        %     register(client_pid, SendPid),
        %     {client_pid, HostName} ! search_by_name(DbList, AnyName),
        %     unregister(client_pid),
        %     db_hand_loop(DbList);
        stop_handle ->
            ok
    end.
