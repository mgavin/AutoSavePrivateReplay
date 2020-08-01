#include "pch.h"
#include "AutoSavePrivateReplay.h"


BAKKESMOD_PLUGIN(AutoSavePrivateReplay, "Automatically saves private replays",
    plugin_version, PLUGINTYPE_FREEPLAY)

void AutoSavePrivateReplay::onLoad()
{
    auto enableVar = cvarManager->registerCvar(
        "ranked_autosavereplay_private_only", "0", "only save private replays");
    enableVar.addOnValueChanged([this](std::string, CVarWrapper cvar) {
        enable(cvar.getBoolValue());
        });;
    enable(enableVar.getBoolValue());
}

void AutoSavePrivateReplay::enable(bool enabled) {
   if (enabled) {
        // hooks on a starting game 
        gameWrapper->HookEvent(
            "Function GameEvent_Soccar_TA.Active.StartRound",
            std::bind(&AutoSavePrivateReplay::startGame, this));
        cvarManager->log("hooked events");
    }
    else {
        gameWrapper->UnhookEventPost(
            "Function GameEvent_Soccar_TA.WaitingForPlayers.BeginState");
        gameWrapper->UnhookEventPost(
            "Function Engine.PlayerInput.InitInputSystem");
    }
}

void AutoSavePrivateReplay::startGame() {
    if (!gameWrapper->IsInOnlineGame()) {
        cvarManager->log("not in online game"); 
        return;
    }

    auto game = gameWrapper->GetOnlineGame();

    if (game.IsNull()) {
        cvarManager->log("null game");
        return;
    }

    auto playlist = game.GetPlaylist();

    if (playlist.memory_address == NULL) {
        cvarManager->log("null playlist");
        return;
    }

    cvarManager->log(std::to_string(playlist.GetPlaylistId()));
    // playlist id 6 is private match
    if (playlist.GetPlaylistId() == 6) {
        cvarManager->executeCommand("ranked_autosavereplay_all 1");
    } else {
        cvarManager->executeCommand("ranked_autosavereplay_all 0");
    }
}

void AutoSavePrivateReplay::onUnload()
{
}