# MultiAddonManager

A MetaMod plugin that allows you to use multiple workshop addons at once and have clients download them.

## Commands
- `mm_extra_addons <ids>`
The workshop IDs of extra addons separated by commas, addons will be downloaded (if not present) and mounted (e.g. "3090239773,3070231528").
Because the download can take time, it's advised to have the addon ready before running maps so content can be precached.
If you edit this during a map, you must change map so clients can also get the new addons

- `mm_extra_addons_timeout <seconds>` How long until clients are timed out in between connects for extra addons

- `mm_download_addon <id>` Download an addon manually
- `mm_print_searchpaths` Print all the search paths currently mounted by the server
## Installation

- Install [Metamod](https://cs2.poggu.me/metamod/installation/)
- Download the [latest release package](https://github.com/Source2ZE/MultiAddonManager/releases/latest) for your OS
- Extract the package contents into `game/csgo` on your server
- Edit the config file at `game/csgo/cfg/multiaddonmanager/multiaddonmanager.cfg`
