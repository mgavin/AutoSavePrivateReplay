#include "pch.h"
#include "AutoSavePrivateReplay.h"


BAKKESMOD_PLUGIN(AutoSavePrivateReplay, "Automatically saves replays of selected playlists",
    plugin_version, PLUGINTYPE_FREEPLAY)


bool pluginEnabled = false;

std::map<int, bool> enabledPlaylists;

const std::map<int, std::string> playlistStrings = {
    {1, "duel"},
    {2, "doubles"},
    {3, "standard"},
    {4, "chaos"},
    {6, "privatematch"},
    {7, "offlineseason"},
    {8, "offlinesplitscreen"},
    {9, "training"},
    {10, "rankedduel"},
    {11, "rankeddoubles"},
    {12, "rankedsolostandard"},
    {13, "rankedstandard"},
    {14, "mutatormashup"},
    {15, "snowday"},
    {16, "rocketlabs"},
    {17, "hoops"},
    {18, "rumble"},
    {19, "workshop"},
    {20, "trainingeditor"},
    {21, "customtraining"},
    {22, "customtournament"},
    {23, "dropshot"},
    {27, "rankedhoops"},
    {28, "rankedrumble"},
    {29, "rankeddropshot"},
    {30, "rankedsnowday"},
    {31, "hauntedball"},
    {32, "beachball"},
    {33, "rugby"},
    {34, "tournament"},
    {37, "rumshot"},
    {38, "godball"},
    {41, "boomerball"},
    {43, "godballdoubles"}
};

enum Playlist
{
    DUEL = 1,
    DOUBLES = 2,
    STANDARD = 3,
    CHAOS = 4,
    PRIVATEMATCH = 6,
    OFFLINESEASON = 7,
    OFFLINESPLITSCREEN = 8,
    TRAINING = 9,
    RANKEDDUEL = 10,
    RANKEDDOUBLES = 11,
    RANKEDSOLOSTANDARD = 12,
    RANKEDSTANDARD = 13,
    MUTATORMASHUP = 14,
    SNOWDAY = 15,
    ROCKETLABS = 16,
    HOOPS = 17,
    RUMBLE = 18,
    WORKSHOP = 19,
    TRAININGEDITOR = 20,
    CUSTOMTRAINING = 21,
    OLDTOURNAMENT = 22,
    DROPSHOT = 23,
    RANKEDHOOPS = 27,
    RANKEDRUMBLE = 28,
    RANKEDDROPSHOT = 29,
    RANKEDSNOWDAY = 30,
    HAUNTEDBALL = 31,
    BEACHBALL = 32,
    RUGBY = 33,
    TOURNAMENT = 34,
    RUMSHOT = 37,
    GODBALL = 38,
    BOOMERBALL = 41,
    GODBALLDOUBLES = 43
};

void AutoSavePrivateReplay::onLoad()
{
    auto enablePluginVar = cvarManager->registerCvar(
        "ranked_autosavereplay_playlists", "0", "save replays from specified playlists");
    enablePluginVar.addOnValueChanged([this](std::string, CVarWrapper cvar) {
        enable(cvar.getBoolValue());
        });

    for (auto playlistPair : playlistStrings) {
        auto enableVar = cvarManager->registerCvar(
            "ranked_autosavereplay_" + playlistPair.second, "0", "save replays from " + playlistPair.second);
        enableVar.addOnValueChanged([this, playlistPair](std::string, CVarWrapper cvar) {
            enablePlaylist(playlistPair.first, cvar.getBoolValue());
            });
    }

    for (auto playlistPair : playlistStrings) {
        enabledPlaylists.emplace(playlistPair.first, false);
    }
}

void AutoSavePrivateReplay::enablePlaylist(int playlistID, bool enabled) {
    auto enabledPtr = enabledPlaylists.find(playlistID);

    if (enabledPtr != enabledPlaylists.end()) {
        enabledPtr->second = enabled;
    }
    else {
        cvarManager->log("missing playlist: " + std::to_string(playlistID));
        return;
    }
}

void AutoSavePrivateReplay::enable(bool enabled) {
   if (enabled) {
        // hooks on a starting game 
        gameWrapper->HookEventPost(
            "Function GameEvent_Soccar_TA.Active.StartRound",
            std::bind(&AutoSavePrivateReplay::startGame, this));
        cvarManager->log("hooked events");
    } else {
        gameWrapper->UnhookEventPost(
            "Function GameEvent_Soccar_TA.Active.StartRound");
    }
}

void AutoSavePrivateReplay::startGame() {
    auto playlistID = getPlaylistID();

    if (playlistID == -1) return;

    cvarManager->log(std::to_string(playlistID));

    auto enabledPtr = enabledPlaylists.find(playlistID);

    bool enabledPlaylist;

    if (enabledPtr != enabledPlaylists.end()) {
        cvarManager->log("enabled playlist: " + std::to_string(playlistID));
        enabledPlaylist = enabledPtr->second;
    }
    else {
        cvarManager->log("missing playlist: " + std::to_string(playlistID));
        return;
    }

    if (enabledPlaylist) {
        cvarManager->executeCommand("ranked_autosavereplay_all 1");
    } else {
        cvarManager->executeCommand("ranked_autosavereplay_all 0");
    }
}

int AutoSavePrivateReplay::getPlaylistID() {
    if (gameWrapper->IsInOnlineGame()) {
        auto game = gameWrapper->GetOnlineGame();

        if (game.IsNull()) {
            cvarManager->log("null game");
            return -1;
        }

        auto playlist = game.GetPlaylist();

        if (playlist.memory_address == NULL) {
            cvarManager->log("null playlist");
            return -1;
        }

        return playlist.GetPlaylistId();
    } else if (gameWrapper->IsInGame()) {
        auto game = gameWrapper->GetGameEventAsServer();

        if (game.IsNull()) {
            cvarManager->log("null game");
            return -1;
        }

        auto playlist = game.GetPlaylist();

        if (playlist.memory_address == NULL) {
            cvarManager->log("null playlist");
            return -1;
        }

        return playlist.GetPlaylistId();
    }

    cvarManager->log("Not in a game");
    return -1;
}

void AutoSavePrivateReplay::onUnload() {
    cvarManager->executeCommand("ranked_autosavereplay_all 0");
}