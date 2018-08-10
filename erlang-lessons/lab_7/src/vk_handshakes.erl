-module (vk_handshakes).

-export ([start_link/0, start/0]).
-export ([make_handshakes/3]).

%% ===================================================================
%% Module callbacks
%% ===================================================================
start_link() ->
    {ok, spawn(?MODULE, start, [])}.

start() ->
    make_handshakes(10, 20, 2).


% Main handshake function. Using env 'access_token' in 'lab_7' application.
% Define it before start.
make_handshakes( _PointIDList, _FinalID, 0) ->
    no_handshakes;
make_handshakes([], _FinalID, _Num) ->
    no_handshakes;
make_handshakes(PointID, FinalID, Num) when is_integer(PointID)->
    {ok, Acc_tok} = application:get_env(lab_7, access_token),
    {ok, {_, _, ReqRaw}} =
        httpc:request("https://api.vk.com/method/users.get?user_ids=" ++
        integer_to_list(PointID) ++ "&v=5.52&access_token=" ++ Acc_tok),
    {Req} = jiffy:decode(ReqRaw),
    ReqList = maps:from_list(Req),
    [{RespList}] = maps:get(<<"response">>, ReqList),
    PointIdInfo = maps:from_list(RespList),
    case make_handshakes([PointIdInfo], FinalID, Num) of
        no_handshakes -> no_handshakes;
        _ ->
            FName = binary:bin_to_list(
                maps:get(<<"first_name">>, PointIdInfo)),
            LName = binary:bin_to_list(
                maps:get(<<"last_name">>, PointIdInfo)),
            io:format("~p ~p~n", [FName, LName])
    end;
make_handshakes([PointID|PointIDTail], FinalID, Num) when (Num > 0)->
    FriendList = get_friends(maps:get(<<"id">>, PointID)),
    case find_user(FinalID, FriendList) of
        false -> case make_handshakes(FriendList, FinalID, Num - 1) of
                no_handshakes ->
                    make_handshakes(PointIDTail, FinalID, Num);
                HandshUser ->
                    FNameUser = binary:bin_to_list(
                        maps:get(<<"first_name">>, HandshUser)),
                    LNameUser = binary:bin_to_list(
                        maps:get(<<"last_name">>, HandshUser)),
                    io:format("~p ~p~n", [FNameUser, LNameUser]),
                    PointID
            end;
        User ->
        FNameUser = binary:bin_to_list(maps:get(<<"first_name">>, User)),
        LNameUser = binary:bin_to_list(maps:get(<<"last_name">>, User)),
        io:format("~p ~p~n", [FNameUser, LNameUser]),
        PointID
    end.



find_user(_UserID, []) ->
    false;
find_user(UserID, [User|UserTail]) ->
    case (UserID == maps:get(<<"id">>, User)) of
        false -> find_user(UserID, UserTail);
        true -> User
    end.

get_friends(UserID) ->
    {ok, Acc_tok} = application:get_env(lab_7, access_token),
    {ok, {_, _, ReqRaw}} =
        httpc:request("https://api.vk.com/method/friends.get?user_id=" ++
        integer_to_list(UserID) ++
        "&v=5.52&fields=first_name,second_name&access_token=" ++ Acc_tok),
    {Req} = jiffy:decode(ReqRaw),
    ReqList = maps:from_list(Req),
    try
        Resp = maps:get(<<"response">>, ReqList),
        {RespList} = Resp,
        RespMap = maps:from_list(RespList),
        FriendList = maps:get(<<"items">>, RespMap),
        make_user_info(UserID, FriendList, [])
    catch
        _:_ -> []
    end.

make_user_info(_ParID, [], OutputMapList) ->
    OutputMapList;
make_user_info(ParentID, FriendList, OutputMapList) ->
    [{UserPropList} | FriendListTail] = FriendList,
    UserPropMap = maps:from_list(UserPropList),
    UserParPropMap = maps:put(<<"par_id">>, ParentID, UserPropMap),
    make_user_info(ParentID, FriendListTail,
        OutputMapList ++ [UserParPropMap]).
