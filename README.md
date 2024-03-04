# MultiAddonManager

A MetaMod plugin that allows you to use multiple workshop addons at once and have clients download them.

## Commands
- `mm_extra_addons <ids>` The workshop IDs of extra addons separated by commas, addons will be downloaded (if not present) and mounted (e.g. "3090239773,3070231528").
  Once downloads are done, the map is automatically reloaded so content can be precached.

- `mm_extra_addons_timeout <seconds>` How long until clients are timed out in between connects for extra addons, timed out clients will reconnect for their current pending download.
- `mm_print_searchpaths` Print all the search paths currently mounted by the server.

- `mm_download_addon <id>` Download an addon manually.

 Both of these commands require a map reload to apply changes.
- `mm_add_addon <id>` Add an addon to the list, but don't mount.
- `mm_remove_addon <id>` Remove an addon from the list, but don't unmount.

## Usage in other MetaMod plugins
- Include the [public header](https://github.com/Source2ZE/MultiAddonManager/blob/main/public/imultiaddonmanager.h).
- Query the interface in `AllPluginsLoaded` like this:
```cpp
IMultiAddonManager *pInterface = (IMultiAddonManager*)g_SMAPI->MetaFactory(MULTIADDONMANAGER_INTERFACE, nullptr, nullptr);
```

## Installation

- Install [Metamod](https://cs2.poggu.me/metamod/installation/)
- Download the [latest release package](https://github.com/Source2ZE/MultiAddonManager/releases/latest) for your OS
- Extract the package contents into `game/csgo` on your server
- Edit the config file at `game/csgo/cfg/multiaddonmanager/multiaddonmanager.cfg`
