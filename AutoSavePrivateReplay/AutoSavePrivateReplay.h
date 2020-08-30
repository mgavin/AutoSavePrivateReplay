#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"

constexpr auto plugin_version = "2.0";

class AutoSavePrivateReplay: public BakkesMod::Plugin::BakkesModPlugin
{

	//Boilerplate
	virtual void onLoad();
	virtual void onUnload();

	void enable(bool enabled);
	void startGame();
	void enablePlaylist(int playlistID, bool enabled);
};

