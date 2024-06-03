#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"

constexpr auto plugin_version = "3.0.0";

class ReplayAutosave : public BakkesMod::Plugin::BakkesModPlugin {
public:
        virtual void onLoad();
        virtual void onUnload();

        void enable(bool enabled);  // just why
        void startGame();
        void enablePlaylist(int playlistID, bool enabled);
        int  getPlaylistID();
};
