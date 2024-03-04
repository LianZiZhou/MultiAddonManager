/**
 * =============================================================================
 * MultiAddonManager
 * Copyright (C) 2024 xen
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <ISmmPlugin.h>
#include <igameevents.h>
#include <sh_vector.h>
#include "utlqueue.h"
#include "utlvector.h"
#include "steam/steam_api_common.h"
#include "steam/isteamugc.h"
#include "imultiaddonmanager.h"

#ifdef _WIN32
#define ROOTBIN "/bin/win64/"
#define GAMEBIN "/csgo/bin/win64/"
#else
#define ROOTBIN "/bin/linuxsteamrt64/"
#define GAMEBIN "/csgo/bin/linuxsteamrt64/"
#endif

// CONVAR_TODO
// Need to replace with actual cvars once available in SDK
#define FAKE_CVAR(name, description, variable_name, variable_type, variable_type_format, variable_default, protect)		\
	CON_COMMAND_F(name, description, FCVAR_LINKED_CONCOMMAND | FCVAR_SPONLY | (protect ? FCVAR_PROTECTED : FCVAR_NONE))	\
	{																													\
		if (args.ArgC() < 2)																							\
			Msg("%s " #variable_type_format "\n", args[0], variable_name);												\
		else																											\
			variable_name = V_StringTo##variable_type(args[1], variable_default);										\
	}

#define FAKE_INT_CVAR(name, description, variable_name, variable_default, protect)										\
	FAKE_CVAR(name, description, variable_name, Int32, %i, variable_default, protect)

#define FAKE_BOOL_CVAR(name, description, variable_name, variable_default, protect)										\
	FAKE_CVAR(name, description, variable_name, Bool, %i, variable_default, protect)

#define FAKE_FLOAT_CVAR(name, description, variable_name, variable_default, protect)									\
	FAKE_CVAR(name, description, variable_name, Float32, %f, variable_default, protect)

 // assumes std::string variable
#define FAKE_STRING_CVAR(name, description, variable_name, protect)														\
	CON_COMMAND_F(name, description, FCVAR_LINKED_CONCOMMAND | FCVAR_SPONLY | (protect ? FCVAR_PROTECTED : FCVAR_NONE))	\
	{																													\
		if (args.ArgC() < 2)																							\
			Msg("%s %s\n", args[0], variable_name.c_str());																\
		else																											\
			variable_name = args[1];																					\
	}

class MultiAddonManager : public ISmmPlugin, public IMetamodListener
{
public:
	bool Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late);
	bool Unload(char *error, size_t maxlen);
	void *OnMetamodQuery(const char *iface, int *ret);
public: //hooks
	void Hook_GameServerSteamAPIActivated();
	void Hook_StartupServer(const GameSessionConfiguration_t &config, ISource2WorldSession *, const char *);
	bool Hook_ClientConnect(CPlayerSlot slot, const char *pszName, uint64 xuid, const char *pszNetworkID, bool unk1, CBufferString *pRejectReason);
	void Hook_GameFrame(bool simulating, bool bFirstTick, bool bLastTick);

	void BuildAddonPath(const char *pszAddon, char *buf, size_t len);
	bool MountAddon(const char *pszAddon, bool bAddToTail);
	bool UnmountAddon(const char *pszAddon);
	bool AddAddon(const char *pszAddon, bool bRefresh);
	bool RemoveAddon(const char *pszAddon, bool bRefresh);
	void DownloadAddon(const char *pszAddon, bool bImportant, bool bForce);
	void PrintDownloadProgress();
	void RefreshAddons(bool bReloadMap);
	void ClearAddons();
	void ReloadMap();
	void SetCurrentWorkshopMap(const char *pszWorkshopID) { m_sCurrentWorkshopMap = pszWorkshopID; }
	void ClearCurrentWorkshopMap() { m_sCurrentWorkshopMap.clear(); }
public:
	const char *GetAuthor();
	const char *GetName();
	const char *GetDescription();
	const char *GetURL();
	const char *GetLicense();
	const char *GetVersion();
	const char *GetDate();
	const char *GetLogTag();

	CUtlVector<std::string> m_ExtraAddons;
	CUtlVector<std::string> m_MountedAddons;
private:
	CUtlVector<PublishedFileId_t> m_ImportantDownloads; // Important addon downloads that will trigger a map reload when finished
	CUtlQueue<PublishedFileId_t> m_DownloadQueue; // Queue of all addon downloads to print progress

	STEAM_GAMESERVER_CALLBACK_MANUAL(MultiAddonManager, OnAddonDownloaded, DownloadItemResult_t, m_CallbackDownloadItemResult);

	// Used when reloading current map
	std::string m_sCurrentWorkshopMap;
};

extern MultiAddonManager g_MultiAddonManager;

PLUGIN_GLOBALVARS();

// Interface to other plugins
class CAddonManagerInterface : IMultiAddonManager
{
public:
	virtual bool AddAddon(const char *pszAddon) override;
	virtual bool RemoveAddon(const char *pszAddon) override;
	virtual bool IsAddonMounted(const char *pszAddon) override;
	virtual void RefreshAddons() override;
	virtual void ClearAddons() override;
};
