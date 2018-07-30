-module(lab_6_1).

-export([business_logic/2, drop_last/1]).

business_logic(start, _) ->
    business_logic(wait, {});

business_logic(wait, _) ->
    receive
        {ReqNode, Filename} ->
            business_logic(open_file, {ReqNode, Filename});
        stop ->
            ok;
        true ->
            business_logic(wait, {})
    end;

business_logic(open_file, {ReqNode, Filename}) ->
    try
        {ok, IoDevice} = file:open(Filename, [read, binary]),
        business_logic(read_file, {ReqNode, IoDevice, []})
    catch
        _:_ ->
            business_logic(alarm, {ReqNode, "Open error"})
    end;

business_logic(read_file, {ReqNode, IoDevice, DataList}) ->
    case file:read_line(IoDevice) of
        {ok, Data} ->
            business_logic(read_file, {ReqNode, IoDevice,
                DataList ++ binary:split(Data, [<<" ">>], [global])});
        eof ->
            business_logic(close_file, {ReqNode, IoDevice, DataList})
    end;

business_logic(close_file, {ReqNode, IoDevice, DataList}) ->
    file:close(IoDevice),
    business_logic(send_mess, {ReqNode, DataList});

business_logic(send_mess, {ReqNode, DataList}) ->
    ReqNode ! {DataList},
    business_logic(wait, {});

business_logic(alarm, {ReqNode, Reason}) ->
    ReqNode ! {error, Reason},
    business_logic(wait, {}).


drop_last([_H|[]]) ->
    [];
drop_last([H|T]) ->
    [H|drop_last(T)].
