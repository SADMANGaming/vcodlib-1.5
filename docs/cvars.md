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
  * Effect: Specifies an additional callback script file that the server will load on startup (without `.gsc` file extension).

* `fs_svrPaks`  
  * Type: String  
  * Default: `""`  
  * Effect: List of server PAKs that will not be downloaded by clients.
