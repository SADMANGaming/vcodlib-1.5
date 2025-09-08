# Newly Added Cvars

* `g_playerEject`  
  * Type: Int  
  * Default: `1`  
  * Effect: If false, players that are close to each other (potentially causing hitbox overlaps) are no longer automatically moved away from each other.

* `jump_height`  
  * Type: Float  
  * Default: `39.0`  
  * Effect: Changes the jump height.

* `jump_slowdownEnable`  
  * Type: Int  
  * Default: `1`  
  * Effect: Enables/disables jump slowdown.

* `sv_allowRcon`  
  * Type: Int  
  * Default: `1`  
  * Effect: If the cvar is set to 0, rcon commands are ignored.

* `sv_cracked`  
  * Type: Int  
  * Default: `0`  
  * Effect: If true, players without a valid CD key are allowed to join the server (with a zero GUID).

* `sv_fixq3fill`  
  * Type: Int  
  * Default: `0`  
  * Effect: Experimental q3fill bugfix.

* `proxy_enableWelcome`  
  * Type: Int  
  * Default: `0`  
  * Effect: If set to 1 it will send "Welcome dear **player name** from **their country**" message in chat.
 
* `proxy_enableAntiVPN`  
  * Type: Int  
  * Default: `0`  
  * Effect: If set to 1 it will kicks VPN players.

* `sv_logHitchWarning`  
  * Type: Int  
  * Default: `1`  
  * Effect: If set to zero it wont send "hitch warning" messages.
 
* `sv_logHeartbeats`  
  * Type: Int  
  * Default: `0`  
  * Effect: If set to zero it wont send "Sending heartbeat to" messages.

* `sv_spectator_noclip`  
  * Type: Int  
  * Default: `0`  
  * Effect: Allows spectators to noclip through walls and objects.

* `sv_connectMessage`  
  * Type: String  
  * Default: `""`  
  * Effect: This message will be shown when a client is trying to join the server.

* `fs_callbacks`  
  * Type: String  
  * Default: `""`  
  * Effect: Defines the relative path to the `.gsc` file (without file suffix) containing script callback methods/functions. If empty, the server uses `maps/mp/gametypes/_callbacksetup`.

* `fs_callbacks_additional`  
  * Type: String  
  * Default: `""`  
  * Effect: Specifies an additional callback script file that the server will load on startup (without `.gsc` file extension. Example: set fs_callbacks_additional "callback_custom").

* `fs_svrPaks`  
  * Type: String  
  * Default: `""`  
  * Effect: List of server PAKs that will not be downloaded by clients.

* `sv_downloadForce`  
  * Type: String  
  * Default: `"0"`  
  * Effect: Force cl_allowdownload cvar on client

* `sv_downloadNotifications`  
  * Type: String  
  * Default: `"0"`  
  * Effect: Shows which file is currently downloaded by the client

* `sv_fastDownload`  
  * Type: String  
  * Default: `"0"`  
  * Effect: Makes UDP download system faster
 
* `discord_logChat`
  * Type: Int
  * Default: `"0"`
  * Effect: If set to 1 it will send game chat messages to Discord.
 
* `discord_logChatURL`
  * Type: String
  * Default: `""`
  * Effect: If set to you'r Discord channel's webhook URL, it will send game chat messages to that channel through webhook.

 * `discord_logTeamSay`
  * Type: Int
  * Default: `"0"`
  * Effect: If set to 1 it will send game team say messages to Discord.

 * `sv_botHook`
  * Type: Int
  * Default: `"0"`
  * Effect: If set to 1 you can use vcodlib GSC bot methods, if set to 0 you can't use vcodlib bot GSC bot methods.
