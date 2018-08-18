-module (vk_mess_handler).

-export ([start_link/0, start_listen/0]).

start_link() ->
    {ok, spawn(?MODULE, start_listen, [])}.

start_listen() ->
    {ok, Group} = application:get_env(group),
    {Server, Key, Ts} = get_lp_server(Group),
    listen(Server, Key, Ts).

listen(Server, Key, Ts) ->
    {NewTs, UpdateList} = get_events(Server, Key, Ts),
    group_event_handler(UpdateList),
    listen(Server, Key, NewTs).


group_event_handler([]) ->
    ok;
group_event_handler([Event| Tail]) ->
    {FromID, Text} = get_info_from_event(Event),
    URIText = uri_encoder:encode(unicode:characters_to_list(Text)),
    Voice = get_voice(URIText),
    URL = get_link_to_upload(FromID),
    {OwnerID, MediaID} = send_file(URL, Voice),
    {ok, _} = send_voice_mess(FromID, OwnerID, MediaID),
    group_event_handler(Tail).

get_lp_server(GroupID) ->
    {ok, VKTok} = application:get_env(vk_token),
    {ok, {_, _, GroupInfo}} = httpc:request(
        "https://api.vk.com/method/groups.getLongPollServer?" ++
        "group_id=" ++ GroupID ++
        "&access_token=" ++ VKTok ++
        "&v=5.80"
    ),
    {[{<<"response">>, {[{<<"key">>,InpK}, {<<"server">>, InpS},
        {<<"ts">>, InpTs}]}}]} = jiffy:decode(GroupInfo),
    {binary_to_list(InpS), binary_to_list(InpK),
        integer_to_list(InpTs)}.

get_events(Server, Key, Ts) ->
    {ok, {_, _, EventReq}} = httpc:request(
        get,
        {Server ++ "?act=a_check" ++
            "&key=" ++ Key ++
            "&ts=" ++ Ts ++
            "&wait=" ++ "90", []},
        [{timeout, 91000}],
        []
    ),
    {[{<<"ts">>, NewTs},{<<"updates">>, UpdateList}]} =
        jiffy:decode(EventReq),
    {binary_to_list(NewTs), UpdateList}.

get_info_from_event(Event) ->
    {[
        {<<"type">>,<<"message_new">>},
        {<<"object">>,
            {[_,
                {<<"from_id">>, FromID},
                _, _, _,
                {<<"text">>, Text},
                _, _, _, _, _, _
            ]}
        },
        _
    ]} = Event,
    {integer_to_list(FromID), Text}.

get_voice(URIText) ->
    {ok, YTok} = application:get_env(yandex_token),
    {ok, {_, _, Voice}} = httpc:request(
        "https://tts.voicetech.yandex.net/generate?" ++
        "key=" ++ YTok ++
        "&text=" ++ URIText ++
        "&format=" ++ "opus" ++
        "&quality=" ++ "lo" ++
        "&lang=" ++ "ru-RU" ++
        "&speaker=" ++ "oksana"
        "&emotion=" ++ "evil"
    ),
    Voice.

get_link_to_upload(ID) ->
    {ok, VKTok} = application:get_env(vk_token),
    {ok, {_, _, Link}} = httpc:request(
        "https://api.vk.com/method/docs.getMessagesUploadServer?" ++
        "peer_id=" ++ ID ++
        "&access_token=" ++ VKTok ++
        "&type=" ++ "audio_message" ++
        "&v=5.80"
    ),
    {[{<<"response">>,
        {[{<<"upload_url">>,UploadURL}]}}]} =
        jiffy:decode(Link),
    binary_to_list(UploadURL).

send_file(URL, Voice) ->
    Boundary = "-------aBBVB450glvjfEoqesb6uuHslekgjslgslLKdfFhnGp[e49adADfd",
    Body = mp_fd:format_multipart_formdata(Boundary, [],
        [{file, "file", Voice}]),
    ContentType = lists:concat(["multipart/form-data; boundary=",
        Boundary]),
    Headers = [{"Content-Length", integer_to_list(length(Body))}],
    {ok, {_, _, FileAnsBin}} = httpc:request(post,
        {URL, Headers, ContentType, Body}, [], []),
    {[{<<"file">>, FileAns}]} = jiffy:decode(FileAnsBin),
    {[{<<"response">>,
        [{[{<<"id">>, MediaID},
        {<<"owner_id">>, OwnerID}| _]}]}]} =
        jiffy:decode(save_doc(binary_to_list(FileAns))),
    {integer_to_list(OwnerID), integer_to_list(MediaID)}.

save_doc(File) ->
    {ok, VKTok} = application:get_env(vk_token),
    {ok, {_, _, SaveLink}} = httpc:request(
        "https://api.vk.com/method/docs.save?" ++
        "&access_token=" ++ VKTok ++
        "&file=" ++ File ++
        "&title=" ++ "voice_mess" ++
        "&v=5.80"
    ),
    SaveLink.

send_voice_mess(FromID, OwnerID, MediaID) ->
    {ok, VKTok} = application:get_env(vk_token),
    httpc:request(
        "https://api.vk.com/method/messages.send?" ++
        "&user_id=" ++ FromID ++
        "&access_token=" ++ VKTok ++
        "&attachment=" ++ "doc" ++ OwnerID ++ "_" ++ MediaID ++
        "&v=5.80"
    ).
