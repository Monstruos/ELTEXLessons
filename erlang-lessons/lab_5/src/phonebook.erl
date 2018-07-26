%% Phone book
-module(phonebook).

-export([get_records_from_phone_book/1, search_by_id/2,
    search_by_name/2, search_by_phone/2, search/3]).


% This function get records from phonebook-file
get_records_from_phone_book(Filename) ->
    case file:read_file(Filename) of
        {ok, Content} ->
            BinRecords = binary:split(Content, [<<"\n">>], [global]),
            TupleRecords = drop_last(set_records(BinRecords)),
            set_maps(TupleRecords);
        {error, Err} ->
            io:format("Error: ~p", [Err])
    end.


% Next 4 functions are used for represent data from file
set_records([]) ->
    [];
set_records([Rec|Tail]) ->
    List = binary:split(Rec, [<<"; ">>], [global]),
    [set_simple_record([X || X <- List]) | set_records(Tail)].

set_simple_record([X, Y, Z]) ->
    [{id, erlang:binary_to_integer(X)},
        {name, binary_to_atom(Y, latin1)},
        {phone, binary_to_atom(Z, latin1)}];
set_simple_record(_) ->
    ok.

drop_last([_H|[]]) ->
    [];
drop_last([H|T]) ->
    [H|drop_last(T)].

set_maps([]) ->
    [];
set_maps([H|T]) ->
    [maps:from_list(H)| set_maps(T)].


% searching by id
search_by_id([], _Id) ->
    [];
search_by_id([Rec|Tail], Id) ->
    case maps:find(id, Rec) of
        {ok, Val} ->
            if
                Val == Id ->
                    [Rec] ++ search_by_id(Tail, Id);
                true ->
                    search_by_id(Tail, Id)
            end;
        error ->
            {error, "not a records"}
    end.

% searching by name
search_by_name([], _Name) ->
    [];
search_by_name([Rec|Tail], Name) ->
    case maps:find(name, Rec) of
        {ok, Val} ->
            if
                Val == Name ->
                    [Rec] ++ search_by_name(Tail, Name);
                true ->
                    search_by_name(Tail, Name)
            end;
        error ->
            {error, "not a records"}
    end.

% searching by phone
search_by_phone([], _Phone) ->
    [];
search_by_phone([Rec|Tail], Phone) ->
    case maps:find(phone, Rec) of
        {ok, Val} ->
            if
                Val == Phone ->
                    [Rec] ++ search_by_phone(Tail, Phone);
                true ->
                    search_by_phone(Tail, Phone)
            end;
        error ->
            {error, "not a records"}
    end.

% universal search
% note: continue search even if records has no such key
search([], _Key, _Value) ->
    [];
search([Rec|Tail], Key, Value) ->
    case maps:find(Key, Rec) of
        {ok, FindVal} ->
            if
                FindVal == Value ->
                    [Rec] ++ search(Tail, Key, Value);
                true ->
                    search(Tail, Key, Value)
            end;
        error ->
            search(Tail, Key, Value)
    end.
