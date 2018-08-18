% Application - group bot for VK that send voice message to user -
% user's text message sended to bot. Using Yandex SpeechKit.
% Before starting, VK, Yandex tokens and GroupID should define into file
% config.txt into root directory of project (JSON-format). Example:
% { "config":
%     {"vk_token": "vk_token",
%     "yandex_token": "y_token",
%     "group": 000000000000}
% }
-module(speaker_app).

-behaviour(application).

%% Application callbacks
-export([start/2, stop/1]).

%% ===================================================================
%% Application callbacks
%% ===================================================================

start(_StartType, _StartArgs) ->
    {ok, BinConfig} = file:read_file("config.txt"),
    {[{<<"config">>,
        {[
            {<<"vk_token">>, VKTBin},
            {<<"yandex_token">>, YTBin},
            {<<"group">>,GroupID}
        ]}
    }]} = jiffy:decode(BinConfig),
    application:set_env(speaker, vk_token, binary_to_list(VKTBin)),
    application:set_env(speaker, yandex_token, binary_to_list(YTBin)),
    application:set_env(speaker, group, integer_to_list(GroupID)),
    speaker_sup:start_link().

stop(_State) ->
    ok.
