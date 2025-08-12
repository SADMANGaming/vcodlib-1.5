#include <signal.h>
#include <arpa/inet.h>
#if COMPILE_LIBCURL == 1
#include <curl/curl.h>
#endif
#include <memory>
#include <tuple>
#include <array>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <cmath>    // fabs, sqrt
#include <unordered_map>

#include "gsc.hpp"



// Stock cvars
cvar_t *com_cl_running;
cvar_t *com_dedicated;
cvar_t *com_logfile;
cvar_t *com_sv_running;
cvar_t *fs_game;
cvar_t *sv_allowDownload;
cvar_t *sv_floodProtect;
cvar_t *sv_fps;
cvar_t *sv_gametype;
cvar_t *sv_mapRotation;
cvar_t *sv_mapRotationCurrent;
cvar_t *sv_maxclients;
cvar_t *sv_maxRate;
cvar_t *sv_pure;
cvar_t *sv_rconPassword;
cvar_t *sv_serverid;
cvar_t *sv_showCommands;
cvar_t *net_ip;
cvar_t *net_port;
cvar_t *sv_wwwDownload;
cvar_t* sv_debugRate;
cvar_t *sv_showAverageBPS;
cvar_t *fs_basegame;

// Custom cvars
cvar_t *sv_cracked;
cvar_t *fs_callbacks_additional;
cvar_t *fs_callbacks;
cvar_t *sv_spectator_noclip;
cvar_t* jump_slowdownEnable;
cvar_t *jump_height;
cvar_t *sv_allowRcon;
cvar_t *fs_svrPaks;
cvar_t *sv_fixq3fill;
cvar_t *g_playerEject;
cvar_t *sv_connectMessage;
cvar_t *sv_connectMessageChallenges;
cvar_t *sv_fastDownload;
cvar_t *sv_downloadNotifications;
cvar_t *sv_downloadForce;
//cvar_t *g_playerCollision;
cvar_t *sv_logHeartbeats;
cvar_t *sv_logHitchWarning;

cvar_t *proxy_enableAntiVPN;
cvar_t *proxy_enableWelcome;

cvar_t *discord_logTeamSay;
cvar_t *discord_logChatURL;
cvar_t *discord_logChat;

cvar_t *g_legacyStyle;

/*cvar_t *player_sprint;
cvar_t *player_sprintMinTime;
cvar_t *player_sprintSpeedScale;
cvar_t *player_sprintTime;*/

cHook *hook_com_init;
cHook *hook_gametype_scripts;
cHook *hook_sv_addoperatorcommands;
cHook *hook_sys_loaddll;
cHook *hook_SV_BotUserMove;
cHook *hook_clientendframe;
cHook *hook_sv_begindownload_f;
cHook *hook_PM_FlyMove;
//cHook *hook_PmoveSingle;
//cHook *hook_ClientSpawn;
cHook *hook_SV_MasterHeartbeat;
cHook *hook_SV_HitchWarning;
cHook* hook_cvar_set2;
cHook *hook_G_Damage;

cHook *hook_ClientBegin;
#if COMPILE_LIBCURL == 1
cHook *hook_G_Say;
#endif
// Stock callbacks
int codecallback_startgametype = 0;
int codecallback_playerconnect = 0;
int codecallback_playerdisconnect = 0;
int codecallback_playerdamage = 0;
int codecallback_playerkilled = 0;

// Custom callbacks
int codecallback_client_spam = 0;
int codecallback_playercommand = 0;
int codecallback_playerconnecting = 0;

// Resume addresses
uintptr_t resume_addr_PM_WalkMove;
uintptr_t resume_addr_PM_SlideMove;

callback_t callbacks[] =
{
    { &codecallback_startgametype, "CodeCallback_StartGameType" }, // g_scr_data.gametype.startupgametype
    { &codecallback_playerconnect, "CodeCallback_PlayerConnect" }, // g_scr_data.gametype.playerconnect
    { &codecallback_playerdisconnect, "CodeCallback_PlayerDisconnect" }, // g_scr_data.gametype.playerdisconnect
    { &codecallback_playerdamage, "CodeCallback_PlayerDamage" }, // g_scr_data.gametype.playerdamage
    { &codecallback_playerkilled, "CodeCallback_PlayerKilled" }, // g_scr_data.gametype.playerkilled

    { &codecallback_client_spam, "CodeCallback_CLSpam"},
    { &codecallback_playercommand, "CodeCallback_PlayerCommand"},
    { &codecallback_playerconnecting, "CodeCallback_PlayerConnecting"}
};



//// Game lib
void* libHandle;
// Objects
gentity_t *g_entities;
level_locals_t *level;
vec3_t *playerMins;
vec3_t *playerMaxs;
pmove_t **pm;
pml_t *pml;
scr_const_t *scr_const;
// Functions
ClientCommand_t ClientCommand;
Scr_IsSystemActive_t Scr_IsSystemActive;
Scr_GetNumParam_t Scr_GetNumParam;
Scr_GetInt_t Scr_GetInt;
Scr_GetString_t Scr_GetString;
Scr_AddBool_t Scr_AddBool;
Scr_AddInt_t Scr_AddInt;
Scr_AddFloat_t Scr_AddFloat;
Scr_AddString_t Scr_AddString;
Scr_AddUndefined_t Scr_AddUndefined;
Scr_AddVector_t Scr_AddVector;
Scr_MakeArray_t Scr_MakeArray;
Scr_AddArray_t Scr_AddArray;
Scr_GetFunction_t Scr_GetFunction;
Scr_GetMethod_t Scr_GetMethod;
Scr_Error_t Scr_Error;
Scr_LoadScript_t Scr_LoadScript;
Scr_GetFunctionHandle_t Scr_GetFunctionHandle;
Scr_ExecEntThread_t Scr_ExecEntThread;
Scr_FreeThread_t Scr_FreeThread;
trap_Argv_t trap_Argv;
va_t va;
VectorNormalize_t VectorNormalize;
BG_AddPredictableEventToPlayerstate_t BG_AddPredictableEventToPlayerstate;
BG_CheckProne_t BG_CheckProne;
BG_PlayAnim_t BG_PlayAnim;
PitchForYawOnNormal_t PitchForYawOnNormal;
AngleDelta_t AngleDelta;
PM_CheckJump_t PM_CheckJump;
PM_AirMove_t PM_AirMove;
PM_Friction_t PM_Friction;
PM_CmdScale_Walk_t PM_CmdScale_Walk;
PM_ClipVelocity_t PM_ClipVelocity;
PM_GetEffectiveStance_t PM_GetEffectiveStance;
PM_Accelerate_t PM_Accelerate;
PM_StepSlideMove_t PM_StepSlideMove;
PM_SetMovementDir_t PM_SetMovementDir;
PM_ViewHeightAdjust_t PM_ViewHeightAdjust;
PM_ClearAimDownSightFlag_t PM_ClearAimDownSightFlag;
PM_GroundSurfaceType_t PM_GroundSurfaceType;
PM_trace_t PM_trace;
PM_AddEvent_t PM_AddEvent;
PM_FootstepType_t PM_FootstepType;
G_ClientStopUsingTurret_t G_ClientStopUsingTurret;
G_EntUnlink_t G_EntUnlink;

G_AddPredictableEvent_t G_AddPredictableEvent;

trap_UnlinkEntity_t trap_UnlinkEntity;
Scr_SetString_t Scr_SetString;
G_SetClientContents_t G_SetClientContents;
player_die_t player_die;
trap_GetUsercmd_t trap_GetUsercmd;
G_SetOrigin_t G_SetOrigin;
SetClientViewAngle_t SetClientViewAngle;
ClientEndFrame_t ClientEndFrame;
ClientThink_real_t ClientThink_real;
BG_PlayerStateToEntityState_t BG_PlayerStateToEntityState;
G_ClientCanSpectateTeam_t G_ClientCanSpectateTeam;
Cmd_FollowCycle_f_t Cmd_FollowCycle_f;
trap_GetArchivedClientInfo_t trap_GetArchivedClientInfo;
StopFollowing_t StopFollowing;
HudElem_UpdateClient_t HudElem_UpdateClient;
AddLeanToPosition_t AddLeanToPosition;
UnGetLeanFraction_t UnGetLeanFraction;
PM_GetLandFactor_t PM_GetLandFactor;
AngleNormalize180Accurate_t AngleNormalize180Accurate;
AngleNormalize180_t AngleNormalize180;
BG_CheckProneValid_t BG_CheckProneValid;
trap_SendServerCommand_t trap_SendServerCommand;
Jump_Set_t Jump_Set;
PM_NoclipMove_t PM_NoclipMove;
StuckInClient_t StuckInClient;
Q_strncpyz_t Q_strncpyz;
//G_Say_t G_Say;
Scr_ExecThread_t Scr_ExecThread;
Scr_AddIString_t Scr_AddIString;
BG_GetWeaponIndexForName_t BG_GetWeaponIndexForName;
BG_GetInfoForWeapon_t BG_GetInfoForWeapon;
BG_GetNumWeapons_t BG_GetNumWeapons;
Scr_GetEntity_t Scr_GetEntity;
Scr_GetType_t Scr_GetType;
vectoangles_t vectoangles;
/*void UCMD_custom_sprint(client_t *cl);

static ucmd_t ucmds[] =
{
    {"userinfo",        SV_UpdateUserinfo_f,     },
    {"disconnect",      SV_Disconnect_f,         },
    {"cp",              SV_VerifyPaks_f,         },
    {"vdr",             SV_ResetPureClient_f,    },
    {"download",        SV_BeginDownload_f,      },
    {"nextdl",          SV_NextDownload_f,       },
    {"stopdl",          SV_StopDownload_f,       },
    {"donedl",          SV_DoneDownload_f,       },
    {"retransdl",       SV_RetransmitDownload_f, },
    {"wwwdl",           SV_wwwDownload_f,        },
    {"sprint",          UCMD_custom_sprint,      },
    {NULL, NULL}
};*/


void custom_Com_Init(char *commandLine)
{
    hook_com_init->unhook();
    void (*Com_Init)(char *commandLine);
    *(int *)&Com_Init = hook_com_init->from;
    Com_Init(commandLine);
    hook_com_init->hook();
    
    // Get references to stock cvars
    com_cl_running = Cvar_FindVar("cl_running");
    com_dedicated = Cvar_FindVar("dedicated");
    com_logfile = Cvar_FindVar("logfile");
    com_sv_running = Cvar_FindVar("sv_running");
    sv_allowDownload = Cvar_FindVar("sv_allowDownload");
    sv_floodProtect = Cvar_FindVar("sv_floodProtect");
    sv_gametype = Cvar_FindVar("g_gametype");
    sv_maxclients = Cvar_FindVar("sv_maxclients");
    sv_mapRotation = Cvar_FindVar("sv_mapRotation");
    sv_mapRotationCurrent = Cvar_FindVar("sv_mapRotationCurrent");
    sv_pure = Cvar_FindVar("sv_pure");
    sv_rconPassword = Cvar_FindVar("rconpassword");
    sv_serverid = Cvar_FindVar("sv_serverid");
    sv_showCommands = Cvar_FindVar("sv_showCommands");
    sv_fps = Cvar_FindVar("sv_fps");
    sv_maxRate = Cvar_FindVar("sv_maxRate");
    sv_showCommands = Cvar_FindVar("sv_showCommands");
    fs_game = Cvar_FindVar("fs_game");
    fs_basegame = Cvar_FindVar("fs_basegame");
    sv_showAverageBPS = Cvar_FindVar("sv_showAverageBPS");
    // net
    net_ip = Cvar_FindVar("net_ip");
    net_port = Cvar_FindVar("net_port");

    // Register custom cvars
    Cvar_Get("vcodlib", "1", CVAR_SERVERINFO);
    sv_wwwDownload = Cvar_Get("sv_wwwDownload", "0", CVAR_SYSTEMINFO | CVAR_ARCHIVE);
    Cvar_Get("sv_wwwBaseURL", "", CVAR_SYSTEMINFO | CVAR_ARCHIVE);
    sv_debugRate = Cvar_Get("sv_debugRate", "0", CVAR_ARCHIVE);
    sv_downloadForce = Cvar_Get("sv_downloadForce", "0", CVAR_ARCHIVE);
    sv_cracked = Cvar_Get("sv_cracked", "0", CVAR_ARCHIVE);
    fs_callbacks = Cvar_Get("fs_callbacks", "", CVAR_ARCHIVE);
    fs_callbacks_additional = Cvar_Get("fs_callbacks_additional", "", CVAR_ARCHIVE);
    sv_spectator_noclip = Cvar_Get("sv_spectator_noclip", "0", CVAR_ARCHIVE);
    jump_slowdownEnable =  Cvar_Get("jump_slowdownEnable", "1", CVAR_SYSTEMINFO | CVAR_ARCHIVE);
    jump_height = Cvar_Get("jump_height", "39.0", CVAR_ARCHIVE);
    sv_allowRcon = Cvar_Get("sv_allowRcon", "1", CVAR_ARCHIVE);
    fs_svrPaks = Cvar_Get("fs_svrPaks", "", CVAR_ARCHIVE);
    sv_fixq3fill = Cvar_Get("sv_fixq3fill", "0", CVAR_ARCHIVE);
    g_playerEject = Cvar_Get("g_playerEject", "1", CVAR_ARCHIVE);
    sv_connectMessage = Cvar_Get("sv_connectMessage", "", CVAR_ARCHIVE);
    sv_connectMessageChallenges = Cvar_Get("sv_connectMessageChallenges", "1", CVAR_ARCHIVE);
    sv_fastDownload = Cvar_Get("sv_fastDownload", "0", CVAR_ARCHIVE);
    sv_downloadNotifications = Cvar_Get("sv_downloadNotifications", "0", CVAR_ARCHIVE);
    sv_logHeartbeats = Cvar_Get("sv_logHeartbeats", "1", CVAR_ARCHIVE);
    sv_logHitchWarning = Cvar_Get("sv_logHitchWarning", "1", CVAR_ARCHIVE);

    proxy_enableAntiVPN = Cvar_Get("proxy_enableAntiVPN", "0", CVAR_ARCHIVE);
    proxy_enableWelcome = Cvar_Get("proxy_enableWelcome", "0", CVAR_ARCHIVE);
    
    discord_logChat = Cvar_Get("discord_logChat", "0", CVAR_ARCHIVE);
    discord_logChatURL = Cvar_Get("discord_logChatURL", "", CVAR_ARCHIVE);
    discord_logTeamSay = Cvar_Get("discord_logTeamSay", "0", CVAR_ARCHIVE);

    g_legacyStyle = Cvar_Get("g_legacyStyle", "0", CVAR_SYSTEMINFO | CVAR_ARCHIVE);

/*    g_playerCollision = Cvar_Get("g_playerCollision", "1", CVAR_ARCHIVE);
    player_sprint = Cvar_Get("player_sprint", "0", CVAR_ARCHIVE);
    player_sprintMinTime = Cvar_Get("player_sprintMinTime", "1.0", CVAR_ARCHIVE);
    player_sprintSpeedScale = Cvar_Get("player_sprintSpeedScale", "1.5", CVAR_ARCHIVE);
    player_sprintTime = Cvar_Get("player_sprintTime", "4.0", CVAR_ARCHIVE);*/
}







// g_LegacyStyle from Raphael
std::map<std::string, std::map<std::string, WeaponProperties>> weapons_properties;

void toggleLegacyStyle(bool enable)
{
    if(enable)
        Cvar_Set2("jump_slowdownEnable", "0", qfalse);
    else
        Cvar_Set2("jump_slowdownEnable", "1", qfalse);

    int id_kar98k_sniper = BG_GetWeaponIndexForName("kar98k_sniper_mp");
    //WeaponDef_t* weapon_kar98k_sniper = BG_GetInfoForWeapon(id_kar98k_sniper);
    weaponinfo_t* weapon_kar98k_sniper = BG_GetInfoForWeapon(id_kar98k_sniper);
    int id_springfield = BG_GetWeaponIndexForName("springfield_mp");
    //WeaponDef_t* weapon_springfield = BG_GetInfoForWeapon(id_springfield);
    weaponinfo_t* weapon_springfield = BG_GetInfoForWeapon(id_springfield);
    int id_mosin_nagant_sniper = BG_GetWeaponIndexForName("mosin_nagant_sniper_mp");
    //WeaponDef_t* weapon_mosin_nagant_sniper = BG_GetInfoForWeapon(id_mosin_nagant_sniper);
    weaponinfo_t* weapon_mosin_nagant_sniper = BG_GetInfoForWeapon(id_mosin_nagant_sniper);
    if (weapon_kar98k_sniper)
    {
        const WeaponProperties* properties_kar98k_sniper = nullptr;
        if(enable)
            properties_kar98k_sniper = &weapons_properties[weapon_kar98k_sniper->name]["legacy"];
        else
            properties_kar98k_sniper = &weapons_properties[weapon_kar98k_sniper->name]["default"];
        weapon_kar98k_sniper->adsTransInTime = properties_kar98k_sniper->adsTransInTime;
        weapon_kar98k_sniper->OOPosAnimLength[0] = 1.0 / (float)weapon_kar98k_sniper->adsTransInTime;
        weapon_kar98k_sniper->adsZoomInFrac = properties_kar98k_sniper->adsZoomInFrac;
        weapon_kar98k_sniper->idleCrouchFactor = properties_kar98k_sniper->idleCrouchFactor;
        weapon_kar98k_sniper->idleProneFactor = properties_kar98k_sniper->idleProneFactor;
        weapon_kar98k_sniper->rechamberWhileAds = properties_kar98k_sniper->rechamberWhileAds;
        weapon_kar98k_sniper->adsViewErrorMin = properties_kar98k_sniper->adsViewErrorMin;
        weapon_kar98k_sniper->adsViewErrorMax = properties_kar98k_sniper->adsViewErrorMax;
    }

    if (weapon_mosin_nagant_sniper)
    {
        const WeaponProperties* properties_mosin_nagant_sniper = nullptr;
        if(enable)
            properties_mosin_nagant_sniper = &weapons_properties[weapon_mosin_nagant_sniper->name]["legacy"];
        else
            properties_mosin_nagant_sniper = &weapons_properties[weapon_mosin_nagant_sniper->name]["default"];
        weapon_mosin_nagant_sniper->reloadAddTime = properties_mosin_nagant_sniper->reloadAddTime;
        weapon_mosin_nagant_sniper->adsTransInTime = properties_mosin_nagant_sniper->adsTransInTime;
        weapon_mosin_nagant_sniper->OOPosAnimLength[0] = 1.0 / (float)weapon_mosin_nagant_sniper->adsTransInTime;
        weapon_mosin_nagant_sniper->adsZoomInFrac = properties_mosin_nagant_sniper->adsZoomInFrac;
        weapon_mosin_nagant_sniper->idleCrouchFactor = properties_mosin_nagant_sniper->idleCrouchFactor;
        weapon_mosin_nagant_sniper->idleProneFactor = properties_mosin_nagant_sniper->idleProneFactor;
        weapon_mosin_nagant_sniper->rechamberWhileAds = properties_mosin_nagant_sniper->rechamberWhileAds;
        weapon_mosin_nagant_sniper->adsViewErrorMin = properties_mosin_nagant_sniper->adsViewErrorMin;
        weapon_mosin_nagant_sniper->adsViewErrorMax = properties_mosin_nagant_sniper->adsViewErrorMax;
    }

    if (weapon_springfield)
    {
        const WeaponProperties* properties_springfield = nullptr;
        if(enable)
            properties_springfield = &weapons_properties[weapon_springfield->name]["legacy"];
        else
            properties_springfield = &weapons_properties[weapon_springfield->name]["default"];
        weapon_springfield->adsTransInTime = properties_springfield->adsTransInTime;
        weapon_springfield->OOPosAnimLength[0] = 1.0 / (float)weapon_springfield->adsTransInTime;
        weapon_springfield->adsZoomInFrac = properties_springfield->adsZoomInFrac;
        weapon_springfield->idleCrouchFactor = properties_springfield->idleCrouchFactor;
        weapon_springfield->idleProneFactor = properties_springfield->idleProneFactor;
        weapon_springfield->rechamberWhileAds = properties_springfield->rechamberWhileAds;
        weapon_springfield->adsViewErrorMin = properties_springfield->adsViewErrorMin;
        weapon_springfield->adsViewErrorMax = properties_springfield->adsViewErrorMax;
    }
}


void custom_Cvar_Set2(const char *var_name, const char *value, qboolean force)
{
    bool check_g_legacyStyle = false;
    bool g_legacyStyle_before;
    bool g_legacyStyle_after;

    if(com_sv_running != NULL && com_sv_running->integer)
    {
        if(!strcasecmp(var_name, g_legacyStyle->name))
        {
            check_g_legacyStyle = true;
            g_legacyStyle_before = g_legacyStyle->integer ? true : false;
        }
    }
    
    hook_cvar_set2->unhook();
    cvar_t* (*Cvar_Set2)(const char *var_name, const char *value, qboolean force);
    *(int *)&Cvar_Set2 = hook_cvar_set2->from;

    if(check_g_legacyStyle)
    {
        cvar_t* var = Cvar_Set2(var_name, value, force);
        if(var)
        {
            g_legacyStyle_after = var->integer ? true : false;
            if(g_legacyStyle_before != g_legacyStyle_after)
                toggleLegacyStyle(var->integer);
        }
    }
    else
        Cvar_Set2(var_name, value, force);

    hook_cvar_set2->hook();
}








int custom_GScr_LoadGameTypeScript()
{
    unsigned int i;
    char path_for_cb[512] = "maps/mp/gametypes/_callbacksetup";

    hook_gametype_scripts->unhook();
    int (*GScr_LoadGameTypeScript)();
    *(int *)&GScr_LoadGameTypeScript = hook_gametype_scripts->from;
    int ret = GScr_LoadGameTypeScript();
    hook_gametype_scripts->hook();

    if(*fs_callbacks_additional->string)
    {
        if(!Scr_LoadScript(fs_callbacks_additional->string))
            Com_DPrintf("custom_GScr_LoadGameTypeScript: Scr_LoadScript for fs_callbacks_additional cvar failed.\n");
    }
    else
    {
        Com_DPrintf("custom_GScr_LoadGameTypeScript: No custom callback file specified in fs_callbacks_additional cvar.\n");
    }

    if(*fs_callbacks->string)
        strncpy(path_for_cb, fs_callbacks->string, sizeof(path_for_cb));
        
    for (i = 0; i < sizeof(callbacks)/sizeof(callbacks[0]); i++)
    {
        if(!strcmp(callbacks[i].name, "CodeCallback_PlayerCommand")) // Custom callback: PlayerCommand
            *callbacks[i].pos = Scr_GetFunctionHandle(fs_callbacks_additional->string, callbacks[i].name);
//        else if(!strcmp(callbacks[i].name, "CodeCallback_PlayerConnecting")) // Custom callback: 
//            *callbacks[i].pos = Scr_GetFunctionHandle(fs_callbacks_additional->string, callbacks[i].name);
        else
            *callbacks[i].pos = Scr_GetFunctionHandle(path_for_cb, callbacks[i].name);
        
        /*if ( *callbacks[i].pos && g_debugCallbacks->integer )
            Com_Printf("%s found @ %p\n", callbacks[i].name, scrVarPub.programBuffer + *callbacks[i].pos);*/ //TODO: verify scrVarPub_t
    }

    return ret;
}


void hook_ClientCommand(int clientNum)
{
    if(!Scr_IsSystemActive())
        return;

    char* cmd = Cmd_Argv(0);
    if(!strcmp(cmd, "gc"))
        return; // Prevent server crash
      
    if(!codecallback_playercommand)
    {
        ClientCommand(clientNum);
        return;
    }

    stackPushArray();
    int args = Cmd_Argc();
    for(int i = 0; i < args; i++)
    {
        char tmp[MAX_STRINGLENGTH];
        trap_Argv(i, tmp, sizeof(tmp));
        if(i == 1 && tmp[0] >= 20 && tmp[0] <= 22)
        {
            char *part = strtok(tmp + 1, " ");
            while(part != NULL)
            {
                stackPushString(part);
                stackPushArrayLast();
                part = strtok(NULL, " ");
            }
        }
        else
        {
            stackPushString(tmp);
            stackPushArrayLast();
        }
    }
    
    short ret = Scr_ExecEntThread(&g_entities[clientNum], codecallback_playercommand, 1);
    Scr_FreeThread(ret);
}



qboolean FS_svrPak(const char *base)
{
    if (strstr(base, "_svr_"))
        return qtrue;

    if (*fs_svrPaks->string)
    {
        bool isSvrPak = false;
        size_t lenString = strlen(fs_svrPaks->string) +1;
        char* stringCopy = (char*)malloc(lenString);
        strcpy(stringCopy, fs_svrPaks->string);

        const char* separator = ";";
        char* strToken = strtok(stringCopy, separator);

        while (strToken != NULL)
        {
            if (!strcmp(base, strToken))
            {
                isSvrPak = true;
                break;
            }
            strToken = strtok(NULL, separator);
        }

        free(stringCopy);
        if (isSvrPak)
            return qtrue;
    }

    return qfalse;
}

bool shouldServeFile(const char *requestedFilePath)
{
    static char localFilePath[MAX_OSPATH*2+5];
    searchpath_t* search;

    localFilePath[0] = 0;

    for(search = fs_searchpaths; search; search = search->next)
    {
        if(search->pak)
        {
            snprintf(localFilePath, sizeof(localFilePath), "%s/%s.pk3", search->pak->pakGamename, search->pak->pakBasename);
            if(!strcmp(localFilePath, requestedFilePath))
                if(!FS_svrPak(search->pak->pakBasename))
                    return true;
        }
    }
    return false;
}

void custom_SV_BeginDownload_f(client_t *cl)
{
    //// [exploit patch] q3dirtrav
    // See:
    //- https://aluigi.altervista.org/video/q3dirtrav.avi
    //- https://aluigi.altervista.org/poc/q3dirtrav.zip
    //- https://oldforum.aluigi.org/post3479.html#p3479
    
    int args = Cmd_Argc();
    if (args > 1)
    {
        const char* arg1 = Cmd_Argv(1);
        if (!shouldServeFile(arg1))
        {
            char ip[16];
            snprintf(ip, sizeof(ip), "%d.%d.%d.%d",
                cl->netchan.remoteAddress.ip[0],
                cl->netchan.remoteAddress.ip[1],
                cl->netchan.remoteAddress.ip[2],
                cl->netchan.remoteAddress.ip[3]);
            Com_Printf("WARNING: %s (%s) tried to download %s.\n", cl->name, ip, arg1);
            return;
        }
    }
    ////

    hook_sv_begindownload_f->unhook();
    void (*SV_BeginDownload_f)(client_t *cl);
    *(int*)&SV_BeginDownload_f = hook_sv_begindownload_f->from;
    SV_BeginDownload_f(cl);
    hook_sv_begindownload_f->hook();
}

customChallenge_t customChallenge[MAX_CHALLENGES];
customPlayerState_t customPlayerState[MAX_CLIENTS];
void custom_SV_BotUserMove(client_t *client)
{

    hook_SV_BotUserMove->unhook();
    void (*SV_BotUserMove)(client_t *client);
    *(int*)&SV_BotUserMove = hook_SV_BotUserMove->from;
    SV_BotUserMove(client);
    hook_SV_BotUserMove->hook();
    return;
    
    int num;
    usercmd_t ucmd = {0};

    if(client->gentity == NULL)
        return;

    num = client - svs.clients;
    ucmd.serverTime = svs.time;

    playerState_t *ps = SV_GameClientNum(num);
    gentity_t *ent = &g_entities[num];

    if(customPlayerState[num].botWeapon)
        ucmd.weapon = (byte)(customPlayerState[num].botWeapon & 0xFF);
    else
        ucmd.weapon = (byte)(ps->weapon & 0xFF);

    if(ent->client == NULL)
        return;

    if (ent->client->sess.archiveTime == 0)
    {
        ucmd.buttons = customPlayerState[num].botButtons;
        ucmd.wbuttons = customPlayerState[num].botWButtons;
        ucmd.forwardmove = customPlayerState[num].botForwardMove;
        ucmd.rightmove = customPlayerState[num].botRightMove;
        ucmd.upmove = customPlayerState[num].botUpMove;

        VectorCopy(ent->client->sess.cmd.angles, ucmd.angles);
    }

    client->deltaMessage = client->netchan.outgoingSequence - 1;
    SV_ClientThink(client, &ucmd);
}


int custom_ClientEndFrame(gentity_t *ent)
{
    hook_clientendframe->unhook();
    int (*ClientEndFrame)(gentity_t *ent);
    *(int *)&ClientEndFrame = hook_clientendframe->from;
    int ret = ClientEndFrame(ent);
    hook_clientendframe->hook();

    if (ent->client->sess.sessionState == STATE_PLAYING)
    {
        int num = ent - g_entities;

        if(customPlayerState[num].gravity > 0)
        ent->client->ps.gravity = customPlayerState[num].gravity;

        if(customPlayerState[num].speed > 0)
        ent->client->ps.speed = customPlayerState[num].speed;

//        if(customPlayerState[num].sprintActive)
//        ent->client->ps.speed *= player_sprintSpeedScale->value;

    }

    return ret;
}

void custom_G_Damage(gentity_s *self, gentity_s *inflictor, gentity_s *ent, const float *vDir,const float *vPoint, int value, /*int dflags,*/ int meansOfDeath, int hitLoc, int timeOffset)
{
    hook_G_Damage->unhook();
    void (*G_Damage)(gentity_s *self, gentity_s *inflictor, gentity_s *ent, const float *vDir,const float *vPoint, int value, /*int dflags,*/ int meansOfDeath, int hitLoc, int timeOffset);
    *(int*)&G_Damage = hook_G_Damage->from;
    G_Damage(self, inflictor, ent, vDir, vPoint, value, /*dflags,*/ meansOfDeath, hitLoc, timeOffset);
    hook_G_Damage->hook();

    int clientNum = self - g_entities;
    int attackerNum = ent - g_entities;

    if (clientNum >= 0 && clientNum < sv_maxclients->integer &&
        attackerNum >= 0 && attackerNum < sv_maxclients->integer)
    {
        customPlayerState[clientNum].attacker = attackerNum;
        printf("Set attacker for client %d to client %d\n", clientNum, attackerNum);
    }
    else
    {
        printf("Failed to set attacker for client %d\n", clientNum);
    }

}

void custom_PM_FlyMove()
{
    if (sv_spectator_noclip->integer)
    {
        PM_NoclipMove();
        return;
    }

    hook_PM_FlyMove->unhook();
    void (*PM_FlyMove)();
    *(int*)&PM_FlyMove = hook_PM_FlyMove->from;
    PM_FlyMove();
    hook_PM_FlyMove->hook();
}





//////////// SPRINT
/*
void custom_ClientSpawn(gentity_t *ent, const float *spawn_origin, const float *spawn_angles)
{
    hook_ClientSpawn->unhook();
    void (*ClientSpawn)(gentity_t *ent, const float *spawn_origin, const float *spawn_angles);
    *(int*)&ClientSpawn = hook_ClientSpawn->from;
    ClientSpawn(ent, spawn_origin, spawn_angles);
    hook_ClientSpawn->hook();

    int clientNum = ent - g_entities;

    // Reset sprint
    customPlayerState[clientNum].sprintActive = false;
    customPlayerState[clientNum].sprintRequestPending = false;
    customPlayerState[clientNum].sprintTimer = 0;
}


/.* See:
- https://github.com/voron00/CoD2rev_Server/blob/b012c4b45a25f7f80dc3f9044fe9ead6463cb5c6/src/bgame/bg_weapons.cpp#L481
- CoD4 1.7: 080570ae
*./
void PM_UpdateSprint(pmove_t *pmove)
{
    int timerMsec;
    int clientNum;
    float sprint_time;
    float sprint_minTime;
    gentity_t *gentity;
    client_t *client;

    clientNum = pmove->ps->clientNum;
    gentity = &g_entities[clientNum];
    client = &svs.clients[clientNum];
    sprint_time = player_sprintTime->value * 1000.0;
    sprint_minTime = player_sprintMinTime->value * 1000.0;
    
    if (sprint_time > 0)
    {
        if (customPlayerState[clientNum].sprintRequestPending)
        {
            if (client->lastUsercmd.forwardmove != KEY_FORWARD)
            {
                customPlayerState[clientNum].sprintRequestPending = false;
                return;
            }
            
            if ((gentity->client->ps.eFlags & EF_CROUCHING) || (gentity->client->ps.eFlags & EF_PRONE))
            {
                G_AddPredictableEvent(gentity, EV_STANCE_FORCE_STAND, 0);
                return;
            }
        }
        
        if (customPlayerState[clientNum].sprintActive)
        {
            timerMsec = customPlayerState[clientNum].sprintTimer + pml->msec;

            if((gentity->client->ps.eFlags & EF_CROUCHING) || (gentity->client->ps.eFlags & EF_PRONE))
                customPlayerState[clientNum].sprintActive = false;
            if(client->lastUsercmd.forwardmove != KEY_FORWARD)
                customPlayerState[clientNum].sprintActive = false;
        }
        else
            timerMsec = customPlayerState[clientNum].sprintTimer - pml->msec;
        
        if(timerMsec < 0)
            timerMsec = 0;
        customPlayerState[clientNum].sprintTimer = timerMsec;
        
        if (customPlayerState[clientNum].sprintRequestPending)
        {
            if(gentity->s.groundEntityNum == 1023)
                return; // Player is in air, wait for landing
            else if(customPlayerState[clientNum].sprintTimer < (sprint_time - sprint_minTime))
                customPlayerState[clientNum].sprintActive = true; // Allow sprint
            customPlayerState[clientNum].sprintRequestPending = false;
        }
        else if(customPlayerState[clientNum].sprintActive && customPlayerState[clientNum].sprintTimer > sprint_time)
            customPlayerState[clientNum].sprintActive = false; // Reached max time, disable sprint
    }
    else
    {
        customPlayerState[clientNum].sprintActive = false;
        customPlayerState[clientNum].sprintTimer = 0;
    }
}

void custom_PmoveSingle(pmove_t *pmove)
{
    hook_PmoveSingle->unhook();
    void (*PmoveSingle)(pmove_t *pmove);
    *(int*)&PmoveSingle = hook_PmoveSingle->from;
    PmoveSingle(pmove);
    hook_PmoveSingle->hook();

    PM_UpdateSprint(pmove);
}

void UCMD_custom_sprint(client_t *cl)
{
    int clientNum = cl - svs.clients;
    if (!player_sprint->integer)
    {
        std::string message = "e \"";
        message += "Sprint is not enabled on this server.";
        message += "\"";
        SV_SendServerCommand(cl, SV_CMD_CAN_IGNORE, message.c_str());
        return;
    }
    
    if(customPlayerState[clientNum].sprintActive)
        customPlayerState[clientNum].sprintActive = false;
    else if(customPlayerState[clientNum].sprintRequestPending)
        customPlayerState[clientNum].sprintRequestPending = false;
    else
        customPlayerState[clientNum].sprintRequestPending = true;
}

void custom_SV_ExecuteClientCommand(client_t *cl, const char *s, qboolean clientOK)
{
    ucmd_t *u;

    ((void(*)(int))0x080cb8ee)(1); // Unknown function, seems related to scrAnimGlob
    Cmd_TokenizeString(s);

    for (u = ucmds; u->name; u++)
    {
        if (!strcmp(Cmd_Argv(0), u->name))
        {
            u->func(cl);
            break;
        }
    }

    if(clientOK)
        if(!u->name && sv.state == SS_GAME)
            VM_Call(gvm, GAME_CLIENT_COMMAND, cl - svs.clients);
}
*/





/*
void custom_G_SetClientContents(gentity_t *ent)
{
	int id = ent - g_entities;

	/.* New code start: g_playerCollision dvar *./
	if ( !g_playerCollision->integer )
		return;
	/.* New code end *./

	if ( ent->client->noclip == 0 )
	{
		if ( ent->client->ufo == 0 )
		{
			if ( (ent->client->sess).sessionState == STATE_DEAD )
			{
				ent->r.contents = 0;
			}
			else
			{
				ent->r.contents = CONTENTS_BODY;

				/.* New code start: per-player/team collison *./
				if ( customPlayerState[id].overrideContents )
					ent->r.contents = customPlayerState[id].contents;
				/.* New code end *./
			}
		}
		else
		{
			ent->r.contents = 0;
		}
	}
	else
	{
		ent->r.contents = 0;
	}
}*/


qboolean logHeartbeat = qtrue;
void custom_SV_MasterHeartbeat(const char *hbname)
{
	int sending_heartbeat_string_offset = 0x080dfda0;

	if ( logHeartbeat && sv_logHeartbeats->integer != 1 )
	{
		byte disable = 0;
		memcpy((void *)sending_heartbeat_string_offset, &disable, 1);
		logHeartbeat = qfalse;
		Com_DPrintf("Disabled heartbeat logging\n");
	}
	else if ( !logHeartbeat && sv_logHeartbeats->integer == 1 )
	{
		byte enable = 0x53; // "S"
		memcpy((void *)sending_heartbeat_string_offset, &enable, 1);
		logHeartbeat = qtrue;
		Com_DPrintf("Enabled heartbeat logging\n");
	}

	hook_SV_MasterHeartbeat->unhook();
	void (*SV_MasterHeartbeat)(const char *hbname);
	*(int *)&SV_MasterHeartbeat = hook_SV_MasterHeartbeat->from;
	SV_MasterHeartbeat(hbname);
	hook_SV_MasterHeartbeat->hook();
}


qboolean logHitchWarning = qtrue;
void custom_SV_HitchWarning(int param1)
{
	int hitch_warning_string_offset = 0x80d9800;

	if ( logHitchWarning && sv_logHitchWarning->integer != 1 )
	{
		byte disable = 0;
		memcpy((void *)hitch_warning_string_offset, &disable, 1);
		logHitchWarning = qfalse;
//		Com_DPrintf("Disabled hitch warning logging\n");
	}
	else if ( !logHitchWarning && sv_logHeartbeats->integer == 1 )
	{
		byte enable = 0x53; // "S"
		memcpy((void *)hitch_warning_string_offset, &enable, 1);
		logHitchWarning = qtrue;
//		Com_DPrintf("Enabled hitch warning logging\n");
	}

	hook_SV_HitchWarning->unhook();
	void (*SV_HitchWarning)(int param1);
	*(int *)&SV_HitchWarning = hook_SV_HitchWarning->from;
	SV_HitchWarning(param1);
	hook_SV_HitchWarning->hook();
}


const char* hook_AuthorizeState(int arg)
{
    const char* s = Cmd_Argv(arg);
    if(sv_cracked->integer && !strcmp(s, "deny"))
        return "accept";
    return s;
}

#if COMPILE_LIBCURL == 1
#include <iomanip>
std::string escape_json(const std::string& input) {
    std::ostringstream ss;
    for (char c : input) {
        // Skip unwanted control characters completely (like 0x15)
        if ((unsigned char)c < 0x20 && c != '\b' && c != '\f' && c != '\n' && c != '\r' && c != '\t') {
            // skip this char (don’t add to output)
            continue;
        }

        switch (c) {
            case '\"': ss << "\\\""; break;
            case '\\': ss << "\\\\"; break;
            case '\b': ss << "\\b"; break;
            case '\f': ss << "\\f"; break;
            case '\n': ss << "\\n"; break;
            case '\r': ss << "\\r"; break;
            case '\t': ss << "\\t"; break;
            default:
                if ((unsigned char)c < 0x20) {
                    // This handles any remaining control chars (usually won't happen after above filter)
                    ss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)(unsigned char)c;
                } else {
                    ss << c;
                }
        }
    }
    return ss.str();
}




void custom_G_Say(gentity_s *ent, gentity_s *target, int mode, const char *chatText)
{
    hook_G_Say->unhook();
    void (*G_Say)(gentity_s *ent, gentity_s *target, int mode, const char *chatText);
    *(int*)&G_Say = hook_G_Say->from;
    G_Say(ent, NULL, mode, chatText);
    hook_G_Say->hook();

    CURL *curl;
    CURLcode responseCode;
    struct curl_slist *headers = NULL;

    int clientNum = ent - g_entities;
    client_t *cl = &svs.clients[clientNum];
    char* name = cl->name;

//    printf("%s\n", name);

//    printf("%d\n", mode);


    if(discord_logChat->integer == 0)
    return;

    if(mode == 1 && discord_logTeamSay->integer == 0)
    return;



    
    //std::string payload = "{\"content\":\"" + std::string(chatText) + "\"}";
    std::string escapedText = escape_json(chatText);  // Escape special characters
    std::string payload = "{\"content\":\"" + std::string("``") + std::string(name) + std::string(" : ") + std::string(escapedText) + std::string("``") +"\"}";

    curl = curl_easy_init();
    if (curl)
    {
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_URL, discord_logChatURL->string);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 2000L); // Optional: prevent hanging

        responseCode = curl_easy_perform(curl);
        if (responseCode != CURLE_OK)
        {
            Com_Printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(responseCode));
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    else
    {
        Com_Printf("[Discord_Logger]: curl_easy_init() failed\n");
    }
}

// Write callback to store response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out) {
    size_t totalSize = size * nmemb;
    out->append((char*)contents, totalSize);
    return totalSize;
}

// Simple string extractor
std::string extractValue(const std::string& json, const std::string& key) {
    size_t keyPos = json.find("\"" + key + "\"");
    if (keyPos == std::string::npos) return "";

    size_t colon = json.find(':', keyPos);
    if (colon == std::string::npos) return "";

    size_t start = json.find_first_of("\"", colon + 1);
    size_t end = json.find_first_of("\"", start + 1);
    if (start == std::string::npos || end == std::string::npos) return "";

    return json.substr(start + 1, end - start - 1);
}

bool fetchProxyAndCountry(const std::string& ip, std::string& proxy, std::string& country) {

    std::cout << ip << std::endl;
    std::string url = "https://proxycheck.io/v2/" + ip + "?vpn=1&asn=1";
    std::string response;


    CURL* curl = curl_easy_init();
    if (!curl) return false;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.0");

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) return false;

    // Find the section for the IP
    size_t ipSection = response.find(ip);
    if (ipSection == std::string::npos) return false;

    std::string ipData = response.substr(ipSection);
    proxy = extractValue(ipData, "proxy");
    country = extractValue(ipData, "country");

    return !proxy.empty() && !country.empty();
}
#endif





#if COMPILE_LIBCURL == 1
std::string name;
std::string country;
#endif

void custom_SV_DirectConnect(netadr_t from) {

    int maxconnecttime = 10000;

    if (sv_fixq3fill->integer == 1) {
        int connectingCount = 0;
        client_t *duplicateCl = NULL;

        for (int i = 0; i < sv_maxclients->integer; i++) {
            client_t *cl = &svs.clients[i];

            if (cl->state == CS_CONNECTED && NET_CompareBaseAdr(from, cl->netchan.remoteAddress)) {
                int delta = svs.time - cl->lastPacketTime;

                if (delta < maxconnecttime) {
                    connectingCount++;
                    duplicateCl = cl;
                }
            }
        }

        if (connectingCount >= 1 && duplicateCl != NULL) {
            Com_Printf("Rejected duplicate CONNECTING client from IP: %s\n", NET_AdrToString(from));
            NET_OutOfBandPrint(NS_SERVER, from, "Only one CONNECTING client allowed per IP (for a short time).\n");
            SV_DropClient(duplicateCl, "Duplicate IP detected!");
            return;
        }
    }

    char* userinfo = Cmd_Argv(1);
    std::stringstream ss;
    ss << "connect \"" << userinfo << "\"";

    if (*sv_connectMessage->string && sv_connectMessageChallenges->integer) {
        int challengeVal = atoi(Info_ValueForKey(userinfo, "challenge"));
        for (int i = 0; i < MAX_CHALLENGES; i++) {
            if (NET_CompareAdr(from, svs.challenges[i].adr) &&
                svs.challenges[i].challenge == challengeVal &&
                customChallenge[i].ignoredCount < sv_connectMessageChallenges->integer) {

                NET_OutOfBandPrint(NS_SERVER, from, "print\n%s\n", sv_connectMessage->string);
                customChallenge[i].ignoredCount++;
                return;
            }
        }
    }
#if COMPILE_LIBCURL == 1

    std::string proxy;
    std::string adr = NET_AdrToString(from);
    size_t colonPos = adr.find(':');
    std::string ipOnly = (colonPos != std::string::npos) ? adr.substr(0, colonPos) : adr;

    if (fetchProxyAndCountry(ipOnly, proxy, country)) {
        std::cout << "Proxy: " << proxy << "\n";
        std::cout << "Country: " << country << "\n";
    } else {
        std::cerr << "Failed to fetch or parse response.\n";
    }




    if(proxy_enableAntiVPN->integer)
    {
        if (proxy == "yes") {
            Com_Printf("Rejected proxy client from IP: %s (Country: %s)\n", NET_AdrToString(from), country.c_str());
            NET_OutOfBandPrint(NS_SERVER, from, "print\nProxy/VPN connections are not allowed.\n");
            return;
        }
    }

    
    name = Info_ValueForKey(userinfo, "name");

//    std::string msg = std::string("say Welcome dear ^7") + name + "^7 from ^7" + country;
//    const char* finalMsg = msg.c_str();
    
//    Cbuf_ExecuteText(EXEC_APPEND, finalMsg);


#endif
//    stackPushString(ipOnly.c_str());
//    short ret = Scr_ExecEntThread(&g_entities[0], codecallback_playerconnecting, 1);
//    Scr_FreeThread(ret);

    // Allow connection
    SV_DirectConnect(from);
}

void custom_ClientBegin(unsigned int clientNum)
{
    #if COMPILE_LIBCURL == 1
    if(proxy_enableWelcome->integer)
    {
        std::string msg = std::string("say Welcome dear ^7") + name + "^7 from ^7" + country;
        const char* finalMsg = msg.c_str();
    
        Cbuf_ExecuteText(EXEC_APPEND, finalMsg);
    }
    #endif


    hook_ClientBegin->unhook();
    void (*ClientBegin)(unsigned int clientNum);
    *(int*)&ClientBegin = hook_ClientBegin->from;
    ClientBegin(clientNum);
    hook_ClientBegin->hook();
}


/*void custom_SV_DirectConnect(netadr_t from) { // last working

    int maxconnecttime = 10000; 

    if (sv_fixq3fill->integer == 1) {
        int connectingCount = 0;
        client_t *duplicateCl = NULL;

        for (int i = 0; i < sv_maxclients->integer; i++) {
            client_t *cl = &svs.clients[i];

            if (cl->state == CS_CONNECTED && NET_CompareBaseAdr(from, cl->netchan.remoteAddress)) {
                int delta = svs.time - cl->lastPacketTime;

                if (delta < maxconnecttime) {
                    connectingCount++;
                    duplicateCl = cl;  // save the reference
                }
            }
        }

        if (connectingCount >= 1 && duplicateCl != NULL) {
            Com_Printf("Rejected duplicate CONNECTING client from IP: %s\n", NET_AdrToString(from));
            NET_OutOfBandPrint(NS_SERVER, from, "Only one CONNECTING client allowed per IP (for a short time).\n");
            SV_DropClient(duplicateCl, "Duplicate IP detected!");
            return;
        }
    }

    char* userinfo;
    userinfo = Cmd_Argv(1);
    std::stringstream ss;
    ss << "connect \"" << userinfo << "\"";

   if (*sv_connectMessage->string && sv_connectMessageChallenges->integer) {
    int challengeVal = atoi(Info_ValueForKey(userinfo, "challenge"));
    for (int i = 0; i < MAX_CHALLENGES; i++) {
        if (NET_CompareAdr(from, svs.challenges[i].adr) && 
            svs.challenges[i].challenge == challengeVal &&
            customChallenge[i].ignoredCount < sv_connectMessageChallenges->integer) {
            
            NET_OutOfBandPrint(NS_SERVER, from, "print\n%s\n", sv_connectMessage->string);
            customChallenge[i].ignoredCount++;
            return;
        }
    }
}

    // Allow connection
    SV_DirectConnect(from);
}*/


qboolean hook_StuckInClient(gentity_s *self)
{
    if(!g_playerEject->integer)
        return qfalse;
    return StuckInClient(self);
}

void meh()
{
    Com_Printf("meh\n");
}

void getserverip()
{
    const char* ip = net_ip->string;
    int port = net_port->integer;

    if (!ip || !*ip)
        ip = "localhost";

    Com_Printf("Server IP:Port is: %s:%d\n", ip, port);
}


static void vcl_version(void)
{
    printf("\n==============================\n");
    printf("[VCODLIB] > FOR COD 1.5\n");
    printf("==============================\n");
}


static void custom_SV_DumpUser_f() {
    client_t *cl;
    int clientNum;

    if (!com_sv_running->integer) {
        Com_Printf("Server is not running.\n");
        return;
    }

    if (Cmd_Argc() != 2) {
        Com_Printf("Usage: info <clientnum>\n");
        return;
    }

    clientNum = atoi(Cmd_Argv(1));

    if (clientNum < 0 || clientNum >= sv_maxclients->integer) {
        Com_Printf("Invalid client number: %d\n", clientNum);
        return;
    }

    cl = &svs.clients[clientNum];

    if (!cl || cl->state < CS_CONNECTED) {
        Com_Printf("Player %d is not on the server or not connected.\n", clientNum);
        return;
    }

    Com_Printf("userinfo for client %d\n", clientNum);
    Com_Printf("-----------------------\n");
    Info_Print(cl->userinfo);
}


void custom_SV_AddOperatorCommands()
{
    hook_sv_addoperatorcommands->unhook();
    void (*SV_AddOperatorCommands)();
    *(int *)&SV_AddOperatorCommands = hook_sv_addoperatorcommands->from;
    SV_AddOperatorCommands();

    Cmd_AddCommand("vcodlib", vcl_version);
    Cmd_AddCommand("getserverip", getserverip);


    Cmd_RemoveCommand("dumpuser"); // 200 iq shit 
    Cmd_AddCommand("dumpuser", custom_SV_DumpUser_f);
    hook_sv_addoperatorcommands->hook();
}



// See https://nachtimwald.com/2017/04/02/constant-time-string-comparison-in-c/
bool str_iseq(const char *s1, const char *s2)
{
    int             m = 0;
    volatile size_t i = 0;
    volatile size_t j = 0;
    volatile size_t k = 0;

    if (s1 == NULL || s2 == NULL)
        return false;

    while (1) {
        m |= s1[i]^s2[j];

        if (s1[i] == '\0')
            break;
        i++;

        if (s2[j] != '\0')
            j++;
        if (s2[j] == '\0')
            k++;
    }

    return m == 0;
}


void ServerCrash(int sig)
{
    int fd;
    FILE *fp;
    void *array[20];
    size_t size = backtrace(array, 20);

    // Write to crash log
    fp = fopen("./crash.log", "a");
    if (fp)
    {
        fd = fileno(fp);
        fseek(fp, 0, SEEK_END);
        fprintf(fp, "Error: Server crashed with signal 0x%x {%d}\n", sig, sig);
        fflush(fp);
        backtrace_symbols_fd(array, size, fd);
    }
    
    // Write to stderr
    fprintf(stderr, "Error: Server crashed with signal 0x%x {%d}\n", sig, sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    
    system("stty sane");
    exit(1);
}

void hook_SVC_RemoteCommand(netadr_t from, msg_t *msg)
{
    char* password = Cmd_Argv(1);

    if (sv_allowRcon->integer == 0)
	return;

    qboolean badRconPassword = !strlen(sv_rconPassword->string) || !str_iseq(password, sv_rconPassword->string);
    
    if (badRconPassword)
    {
        Com_Printf("Bad rcon from %s\n", NET_AdrToString(from));
        NET_OutOfBandPrint(NS_CLIENT, from, "print\nBad rconpassword");
        return;
    }
    
    SVC_RemoteCommand(from, msg);
}

// See https://github.com/ibuddieat/zk_libcod/blob/dff123fad25d7b46d65685e9bca2111c8946a36e/code/libcod.cpp#L3600
void SV_WriteDownloadErrorToClient(client_t *cl, msg_t *msg, char *errorMessage)
{
    MSG_WriteByte(msg, svc_download);
    MSG_WriteShort(msg, 0);
    MSG_WriteLong(msg, -1);
    MSG_WriteString(msg, errorMessage);
    *cl->downloadName = 0;
}

void custom_SV_WriteDownloadToClient(client_t *cl, msg_t *msg)
{
    int curindex;
    int blksize;
    char downloadNameNoExt[MAX_QPATH];
    char errorMessage[MAX_STRINGLENGTH];

    if(!*cl->downloadName)
        return;
    
    cl->state = CS_CONNECTED;
    cl->rate = 25000;
    cl->snapshotMsec = 50;

    if (sv_wwwDownload->integer && cl->wwwOk)
	{
		if (!cl->wwwFallback)
		{
			SV_WWWRedirectClient(cl, msg);
			return;
		}
	}

    if (!cl->download)
    {
        if (!sv_allowDownload->integer)
        {
            Com_Printf("clientDownload: %d : \"%s\" download disabled\n", cl - svs.clients, cl->downloadName);

            if(sv_pure->integer)
                Com_sprintf(errorMessage, sizeof(errorMessage), "EXE_AUTODL_SERVERDISABLED_PURE\x15%s", cl->downloadName);
            else
                Com_sprintf(errorMessage, sizeof(errorMessage), "EXE_AUTODL_SERVERDISABLED\x15%s", cl->downloadName);

            SV_WriteDownloadErrorToClient(cl, msg, errorMessage);
            return;
        }

        Q_strncpyz(downloadNameNoExt, cl->downloadName, strlen(cl->downloadName) - 3);
        if (FS_idPak(downloadNameNoExt, "main"))
        {
            Com_Printf("clientDownload: %d : \"%s\" cannot download id pk3 files\n", cl - svs.clients, cl->downloadName);
            Com_sprintf(errorMessage, sizeof(errorMessage), "EXE_CANTAUTODLGAMEPAK\x15%s", cl->downloadName);
            SV_WriteDownloadErrorToClient(cl, msg, errorMessage);
            return;
        }

        if ((cl->downloadSize = FS_SV_FOpenFileRead(cl->downloadName, &cl->download)) <= 0)
        {
            Com_Printf("clientDownload: %d : \"%s\" file not found on server\n", cl - svs.clients, cl->downloadName);
            Com_sprintf(errorMessage, sizeof(errorMessage), "EXE_AUTODL_FILENOTONSERVER\x15%s", cl->downloadName);
            SV_WriteDownloadErrorToClient(cl, msg, errorMessage);
            return;
        }

        // Init download
        Com_Printf("clientDownload: %d : beginning \"%s\"\n", cl - svs.clients, cl->downloadName);
        cl->downloadCurrentBlock = cl->downloadClientBlock = cl->downloadXmitBlock = 0;
        cl->downloadCount = 0;
        cl->downloadEOF = qfalse;

        if(sv_downloadNotifications->integer)
            SV_SendServerCommand(0, SV_CMD_CAN_IGNORE, "f \"%s^7 downloads %s\"", cl->name, cl->downloadName);
            
    }
    
    while (cl->downloadCurrentBlock - cl->downloadClientBlock < MAX_DOWNLOAD_WINDOW && cl->downloadSize != cl->downloadCount)
    {
        curindex = (cl->downloadCurrentBlock % MAX_DOWNLOAD_WINDOW);

        blksize = MAX_DOWNLOAD_BLKSIZE;
        if (sv_fastDownload->integer)
            blksize = MAX_DOWNLOAD_BLKSIZE_FAST;
        
        if (!cl->downloadBlocks[curindex])
        {
            // See https://github.com/ibuddieat/zk_libcod/blob/dfdd4ef17508ff8ffbaacb0353a6b736a9707cba/code/libcod.cpp#L3761
            cl->downloadBlocks[curindex] = (unsigned char *)Z_MallocInternal(MAX_DOWNLOAD_BLKSIZE_FAST);
        }
        cl->downloadBlockSize[curindex] = FS_Read(cl->downloadBlocks[curindex], blksize, cl->download);

        if (cl->downloadBlockSize[curindex] < 0)
        {
            // EOF
            cl->downloadCount = cl->downloadSize;
            break;
        }

        cl->downloadCount += cl->downloadBlockSize[curindex];
        // Load in next block
        cl->downloadCurrentBlock++;
    }

    // Check to see if we have eof condition and add the EOF block
    if (cl->downloadCount == cl->downloadSize && !cl->downloadEOF && cl->downloadCurrentBlock - cl->downloadClientBlock < MAX_DOWNLOAD_WINDOW)
    {
        cl->downloadBlockSize[cl->downloadCurrentBlock % MAX_DOWNLOAD_WINDOW] = 0;
        cl->downloadCurrentBlock++;
        cl->downloadEOF = qtrue;
    }

    if(cl->downloadClientBlock == cl->downloadCurrentBlock)
        return; // Nothing to transmit

    if (cl->downloadXmitBlock == cl->downloadCurrentBlock)
    {
        // FIXME: See https://github.com/id-Software/RTCW-MP/blob/937b209a3c14857bea09a692545c59ac1a241275/src/server/sv_client.c#L962
        if(svs.time - cl->downloadSendTime > 1000)
            cl->downloadXmitBlock = cl->downloadClientBlock;
        else
            return;
    }

    // Send current block
    curindex = (cl->downloadXmitBlock % MAX_DOWNLOAD_WINDOW);

    MSG_WriteByte(msg, svc_download);
    MSG_WriteShort(msg, cl->downloadXmitBlock);

    // Block zero contains file size
    if(cl->downloadXmitBlock == 0)
        MSG_WriteLong(msg, cl->downloadSize);

    MSG_WriteShort(msg, cl->downloadBlockSize[curindex]);

    // Write the block
    if(cl->downloadBlockSize[curindex])
        MSG_WriteData(msg, cl->downloadBlocks[curindex], cl->downloadBlockSize[curindex]);

    Com_DPrintf("clientDownload: %d : writing block %d\n", cl - svs.clients, cl->downloadXmitBlock);

    // Move on to the next block
    // It will get sent with next snapshot
    cl->downloadXmitBlock++;
    cl->downloadSendTime = svs.time;
    
}

// See https://github.com/voron00/CoD2rev_Server/blob/b012c4b45a25f7f80dc3f9044fe9ead6463cb5c6/src/server/sv_snapshot_mp.cpp#L686
// FIXME: receiving as client_t* makes download slow
static int SV_RateMsec(client_t client, int messageSize)
{
    int rate;
    int rateMsec;
    
    if(messageSize > 1500)
        messageSize = 1500;

    rate = client.rate;
    if (sv_maxRate->integer)
    {
        if(sv_maxRate->integer < 1000)
            Cvar_Set("sv_MaxRate", "1000");

        if(sv_maxRate->integer < rate)
            rate = sv_maxRate->integer;
    }

    rateMsec = ((messageSize + HEADER_RATE_BYTES) * 1000) / rate;
    if(sv_debugRate->integer)
        Com_Printf("It would take %ims to send %i bytes to client %s (rate %i)\n", rateMsec, messageSize, client.name, client.rate);

    return rateMsec;
}

void custom_SV_SendClientMessages(void)
{
    int i;
    client_t *cl;
    int numclients = 0;

    sv.bpsTotalBytes = 0;
    sv.ubpsTotalBytes = 0;

    for (i = 0; i < sv_maxclients->integer; i++)
    {
        cl = &svs.clients[i];

        if(!cl->state)
            continue;
        if(svs.time < cl->nextSnapshotTime)
            continue;

        numclients++;

        if (sv_fastDownload->integer && cl->download)
        {
            for (int j = 0; j < 1 + ((sv_fps->integer / 20) * MAX_DOWNLOAD_WINDOW); j++)
            {
                while (cl->netchan.unsentFragments)
                {
                    cl->nextSnapshotTime = svs.time + SV_RateMsec(*cl, cl->netchan.unsentLength - cl->netchan.unsentFragmentStart);
                    SV_Netchan_TransmitNextFragment(&cl->netchan);
                }
                SV_SendClientSnapshot(cl);
            }
        }
        else
        {
            if (cl->netchan.unsentFragments)
            {
                cl->nextSnapshotTime = svs.time + SV_RateMsec(*cl, cl->netchan.unsentLength - cl->netchan.unsentFragmentStart);
                SV_Netchan_TransmitNextFragment(&cl->netchan);
                continue;
            }
            SV_SendClientSnapshot(cl);
        }
    }

    if (sv_showAverageBPS->integer && numclients > 0)
    {
        float ave = 0, uave = 0;

        for (i = 0; i < MAX_BPS_WINDOW - 1; i++)
        {
            sv.bpsWindow[i] = sv.bpsWindow[i + 1];
            ave += sv.bpsWindow[i];

            sv.ubpsWindow[i] = sv.ubpsWindow[i + 1];
            uave += sv.ubpsWindow[i];
        }

        sv.bpsWindow[MAX_BPS_WINDOW - 1] = sv.bpsTotalBytes;
        ave += sv.bpsTotalBytes;

        sv.ubpsWindow[MAX_BPS_WINDOW - 1] = sv.ubpsTotalBytes;
        uave += sv.ubpsTotalBytes;

        if(sv.bpsTotalBytes >= sv.bpsMaxBytes)
            sv.bpsMaxBytes = sv.bpsTotalBytes;

        if(sv.ubpsTotalBytes >= sv.ubpsMaxBytes)
            sv.ubpsMaxBytes = sv.ubpsTotalBytes;

        sv.bpsWindowSteps++;

        if (sv.bpsWindowSteps >= MAX_BPS_WINDOW)
        {
            float comp_ratio;

            sv.bpsWindowSteps = 0;

            ave = ave / (float)MAX_BPS_WINDOW;
            uave = uave / (float)MAX_BPS_WINDOW;

            comp_ratio = (1 - ave / uave) * 100.f;
            sv.ucompAve += comp_ratio;
            sv.ucompNum++;

            Com_DPrintf("bpspc(%2.0f) bps(%2.0f) pk(%i) ubps(%2.0f) upk(%i) cr(%2.2f) acr(%2.2f)\n",
                        ave / (float)numclients,
                        ave,
                        sv.bpsMaxBytes,
                        uave,
                        sv.ubpsMaxBytes,
                        comp_ratio,
                        sv.ucompAve / sv.ucompNum);
        }
    }
}

void custom_SV_SendMessageToClient(msg_t *msg, client_t *client)
{
    byte svCompressBuf[MAX_MSGLEN];
    int compressedSize;
    int rateMsec;
    
    memcpy(svCompressBuf, msg->data, 4);
    compressedSize = MSG_WriteBitsCompress(msg->data + 4, svCompressBuf + 4, msg->cursize - 4) + 4;
    if (client->dropReason)
    {
        SV_DropClient(client, client->dropReason);
    }
    client->frames[client->netchan.outgoingSequence & PACKET_MASK].messageSize = compressedSize;
    client->frames[client->netchan.outgoingSequence & PACKET_MASK].messageSent = svs.time;
    client->frames[client->netchan.outgoingSequence & PACKET_MASK].messageAcked = -1;
    SV_Netchan_Transmit(client, svCompressBuf, compressedSize);

    if (client->netchan.remoteAddress.type == NA_LOOPBACK || Sys_IsLANAddress(client->netchan.remoteAddress)
        || (sv_fastDownload->integer && client->download))
    {
        client->nextSnapshotTime = svs.time - 1;
        return;
    }

    rateMsec = SV_RateMsec(*client, compressedSize);
    if (rateMsec < client->snapshotMsec)
    {
        rateMsec = client->snapshotMsec;
        client->rateDelayed = qfalse;
    }
    else
    {
        client->rateDelayed = qtrue;
    }
    client->nextSnapshotTime = svs.time + rateMsec;
    if (client->state != CS_ACTIVE)
    {
        if (!*client->downloadName && client->nextSnapshotTime < svs.time + 1000)
        {
            client->nextSnapshotTime = svs.time + 1000;
        }
    }
    sv.bpsTotalBytes += compressedSize;
}

void custom_SV_SendClientGameState(client_t *client)
{
    int start;
    entityState_t /**base,*/ nullstate;
    msg_t msg;
    byte msgBuffer[MAX_MSGLEN];
    int clientNum = client - svs.clients;
    
    while(client->state != CS_FREE && client->netchan.unsentFragments)
        SV_Netchan_TransmitNextFragment(&client->netchan);

    Com_DPrintf("SV_SendClientGameState() for %s\n", client->name);
    Com_DPrintf("Going from CS_CONNECTED to CS_PRIMED for %s\n", client->name);

    client->state = CS_PRIMED;
    client->pureAuthentic = 0;
    client->gamestateMessageNum = client->netchan.outgoingSequence;

    // Init/reset custom player state
    customPlayerState[clientNum] = {};

    // Restore user-provided rate and snaps after download
    SV_UserinfoChanged(client);
    
    MSG_Init(&msg, msgBuffer, sizeof(msgBuffer));
    MSG_WriteLong(&msg, client->lastClientCommand);
    SV_UpdateServerCommandsToClient(client, &msg);
    MSG_WriteByte(&msg, svc_gamestate);
    MSG_WriteLong(&msg, client->reliableSequence);
    
    for (start = 0; start < MAX_CONFIGSTRINGS; start++)
    {
        if (sv.configstrings[start][0])
        {
            MSG_WriteByte(&msg, svc_configstring);
            MSG_WriteShort(&msg, start);
            std::string csCopy(sv.configstrings[start]);
            if (start == 1)
            {

                if (!sv_allowDownload->integer)
                {
                    /*
                    If client has cl_allowDownload enabled, but sv_allowDownload is disabled, client will attempt to download anyway, and then fail joining.
                    To fix this, if sv_allowDownload is disabled, write cl_allowDownload disabled on client, in case is enabled.
                    */
                   
                    csCopy.append("\\cl_allowDownload\\0");
                }
                else
                {
                    if (sv_downloadForce->integer)
                    {
                        csCopy.append("\\cl_allowDownload\\1");
                    }
                }
            }

            MSG_WriteBigString(&msg, csCopy.c_str());
        }
    }
    
    // TODO: clean
    memset(&nullstate, 0, sizeof(nullstate));
    int *base = (int*)(0x836DC40); // probably unk_836DC40?
    for (start = 0; start < MAX_GENTITIES; start++)
    {
        base += 0x5f;
        //base = &sv.svEntities[start].baseline.s;
        if(!*base)
        //if(!base->number)
            continue;
        MSG_WriteByte(&msg, svc_baseline);
        //MSG_WriteDeltaEntity(&msg, &nullstate, base, qtrue);
        ((void(*)(
            msg_t*,
            struct entityState_s*,
            int*,
            qboolean))0x808149A)(&msg, &nullstate, base, qtrue); //MSG_WriteDeltaEntity
    }
    
    MSG_WriteByte(&msg, svc_EOF);
    MSG_WriteLong(&msg, clientNum);
    MSG_WriteLong(&msg, sv.checksumFeed);
    MSG_WriteByte(&msg, svc_EOF);

    Com_DPrintf("Sending %i bytes in gamestate to client: %i\n", msg.cursize, clientNum);

    SV_SendMessageToClient(&msg, client);
}

void *custom_Sys_LoadDll(const char *name, char *fqpath, int (**entryPoint)(int, ...), int (*systemcalls)(int, ...))
{
    hook_sys_loaddll->unhook();
    void *(*Sys_LoadDll)(const char *name, char *fqpath, int (**entryPoint)(int, ...), int (*systemcalls)(int, ...));
    *(int*)&Sys_LoadDll = hook_sys_loaddll->from;
    libHandle = Sys_LoadDll(name, fqpath, entryPoint, systemcalls);
    hook_sys_loaddll->hook();

    //// Unprotect game.mp.i386.so
    // See https://github.com/xtnded/codextended/blob/855df4fb01d20f19091d18d46980b5fdfa95a712/src/librarymodule.c#L55
    char libPath[512];
    char buf[512];
    char flags[4];
    void *low, *high;
    FILE *fp;
    
    if (*fs_game->string)
        sprintf(libPath, "%s/game.mp.i386.so", fs_game->string);
    else if (*fs_basegame->string)
            sprintf(libPath, "%s/game.mp.i386.so", fs_basegame->string);
    else
        sprintf(libPath, "main/game.mp.i386.so");
    
    fp = fopen("/proc/self/maps", "r");
    if(!fp)
        return 0;

    while (fgets(buf, sizeof(buf), fp))
    {
        if(!strstr(buf, libPath))
            continue;
        if(sscanf (buf, "%p-%p %4c", &low, &high, flags) != 3)
            continue;
        mprotect((void *)low, (int)high-(int)low, PROT_READ | PROT_WRITE | PROT_EXEC);
    }
    fclose(fp);
    ////

    //// Objects
    g_entities = (gentity_t*)dlsym(libHandle, "g_entities");
    level = (level_locals_t*)dlsym(libHandle, "level");
    playerMins = (vec3_t*)dlsym(libHandle, "playerMins");
    playerMaxs = (vec3_t*)dlsym(libHandle, "playerMaxs");
    pm = (pmove_t**)dlsym(libHandle, "pm");
    pml = (pml_t*)dlsym(libHandle, "pml");
    scr_const = (scr_const_t*)dlsym(libHandle, "scr_const");

    ////

    //// Functions
    ClientCommand = (ClientCommand_t)dlsym(libHandle, "ClientCommand");
    Scr_IsSystemActive = (Scr_IsSystemActive_t)dlsym(libHandle, "Scr_IsSystemActive");
    Scr_GetFunctionHandle = (Scr_GetFunctionHandle_t)dlsym(libHandle, "Scr_GetFunctionHandle");
    Scr_GetNumParam = (Scr_GetNumParam_t)dlsym(libHandle, "Scr_GetNumParam");
    Scr_GetFunction = (Scr_GetFunction_t)dlsym(libHandle, "Scr_GetFunction");
    Scr_GetMethod = (Scr_GetMethod_t)dlsym(libHandle, "Scr_GetMethod");
    Scr_Error = (Scr_Error_t)dlsym(libHandle, "Scr_Error");
    Scr_GetInt = (Scr_GetInt_t)dlsym(libHandle, "Scr_GetInt");
    Scr_GetString = (Scr_GetString_t)dlsym(libHandle, "Scr_GetString");
    Scr_AddBool = (Scr_AddBool_t)dlsym(libHandle, "Scr_AddBool");
    Scr_AddInt = (Scr_AddInt_t)dlsym(libHandle, "Scr_AddInt");
    Scr_AddFloat = (Scr_AddFloat_t)dlsym(libHandle, "Scr_AddFloat");
    Scr_AddString = (Scr_AddString_t)dlsym(libHandle, "Scr_AddString");
    Scr_AddUndefined = (Scr_AddUndefined_t)dlsym(libHandle, "Scr_AddUndefined");
    Scr_AddVector = (Scr_AddVector_t)dlsym(libHandle, "Scr_AddVector");
    Scr_MakeArray = (Scr_MakeArray_t)dlsym(libHandle, "Scr_MakeArray");
    Scr_AddArray = (Scr_AddArray_t)dlsym(libHandle, "Scr_AddArray");
    Scr_LoadScript = (Scr_LoadScript_t)dlsym(libHandle, "Scr_LoadScript");
    Scr_ExecEntThread = (Scr_ExecEntThread_t)dlsym(libHandle, "Scr_ExecEntThread");
    Scr_FreeThread = (Scr_FreeThread_t)dlsym(libHandle, "Scr_FreeThread");
    trap_Argv = (trap_Argv_t)dlsym(libHandle, "trap_Argv");
    va = (va_t)dlsym(libHandle, "va");
    VectorNormalize = (VectorNormalize_t)dlsym(libHandle, "VectorNormalize");
    BG_AddPredictableEventToPlayerstate = (BG_AddPredictableEventToPlayerstate_t)dlsym(libHandle, "BG_AddPredictableEventToPlayerstate");
    BG_CheckProne = (BG_CheckProne_t)dlsym(libHandle, "BG_CheckProne");
    BG_PlayAnim = (BG_PlayAnim_t)dlsym(libHandle, "BG_PlayAnim");
    PitchForYawOnNormal = (PitchForYawOnNormal_t)dlsym(libHandle, "PitchForYawOnNormal");
    AngleDelta = (AngleDelta_t)dlsym(libHandle, "AngleDelta");
    PM_CheckJump = (PM_CheckJump_t)((int)dlsym(libHandle, "_init") + 0xCB85);
    PM_AirMove = (PM_AirMove_t)((int)dlsym(libHandle, "_init") + 0xD08D);
    PM_Friction = (PM_Friction_t)((int)dlsym(libHandle, "_init") + 0xBCB3);
    PM_CmdScale_Walk = (PM_CmdScale_Walk_t)((int)dlsym(libHandle, "_init") + 0xC28B);
    PM_ClipVelocity = (PM_ClipVelocity_t)dlsym(libHandle, "PM_ClipVelocity");
    PM_GetEffectiveStance = (PM_GetEffectiveStance_t)dlsym(libHandle, "PM_GetEffectiveStance");
    PM_Accelerate = (PM_Accelerate_t)((int)dlsym(libHandle, "_init") + 0xBF62);
    PM_StepSlideMove = (PM_StepSlideMove_t)dlsym(libHandle, "PM_StepSlideMove");
    PM_SetMovementDir = (PM_SetMovementDir_t)((int)dlsym(libHandle, "_init") + 0xC68F);
    PM_ViewHeightAdjust = (PM_ViewHeightAdjust_t)((int)dlsym(libHandle, "_init") + 0xF8A6);
    PM_ClearAimDownSightFlag = (PM_ClearAimDownSightFlag_t)dlsym(libHandle, "PM_ClearAimDownSightFlag");
    PM_GroundSurfaceType = (PM_GroundSurfaceType_t)((int)dlsym(libHandle, "_init") + 0xC9D7);
    PM_trace = (PM_trace_t)dlsym(libHandle, "PM_trace");
    PM_AddEvent = (PM_AddEvent_t)dlsym(libHandle, "PM_AddEvent");
    PM_FootstepType = (PM_FootstepType_t)((int)dlsym(libHandle, "_init") + 0xE25C);
    G_ClientStopUsingTurret = (G_ClientStopUsingTurret_t)dlsym(libHandle, "G_ClientStopUsingTurret");
    G_EntUnlink = (G_EntUnlink_t)dlsym(libHandle, "G_EntUnlink");
    trap_UnlinkEntity = (trap_UnlinkEntity_t)dlsym(libHandle, "trap_UnlinkEntity");
    Scr_SetString = (Scr_SetString_t)dlsym(libHandle, "Scr_SetString");
    G_SetClientContents = (G_SetClientContents_t)dlsym(libHandle, "G_SetClientContents");
    player_die = (player_die_t)dlsym(libHandle, "player_die");
    trap_GetUsercmd = (trap_GetUsercmd_t)dlsym(libHandle, "trap_GetUsercmd");
    G_SetOrigin = (G_SetOrigin_t)dlsym(libHandle, "G_SetOrigin");
    SetClientViewAngle = (SetClientViewAngle_t)dlsym(libHandle, "SetClientViewAngle");
    ClientEndFrame = (ClientEndFrame_t)dlsym(libHandle, "ClientEndFrame");
    ClientThink_real = (ClientThink_real_t)dlsym(libHandle, "ClientThink_real");
    BG_PlayerStateToEntityState = (BG_PlayerStateToEntityState_t)dlsym(libHandle, "BG_PlayerStateToEntityState");
    G_ClientCanSpectateTeam = (G_ClientCanSpectateTeam_t)dlsym(libHandle, "G_ClientCanSpectateTeam");
    Cmd_FollowCycle_f = (Cmd_FollowCycle_f_t)dlsym(libHandle, "Cmd_FollowCycle_f");
    trap_GetArchivedClientInfo = (trap_GetArchivedClientInfo_t)dlsym(libHandle, "trap_GetArchivedClientInfo");
    StopFollowing = (StopFollowing_t)dlsym(libHandle, "StopFollowing");
    HudElem_UpdateClient = (HudElem_UpdateClient_t)dlsym(libHandle, "HudElem_UpdateClient");
    AddLeanToPosition = (AddLeanToPosition_t)dlsym(libHandle, "AddLeanToPosition");
    UnGetLeanFraction = (UnGetLeanFraction_t)dlsym(libHandle, "UnGetLeanFraction");
    PM_GetLandFactor = (PM_GetLandFactor_t)((int)dlsym(libHandle, "_init") + 0xBC52);
    AngleNormalize180Accurate = (AngleNormalize180Accurate_t)dlsym(libHandle, "AngleNormalize180Accurate");
    AngleNormalize180 = (AngleNormalize180_t)dlsym(libHandle, "AngleNormalize180");
    BG_CheckProneValid = (BG_CheckProneValid_t)dlsym(libHandle, "BG_CheckProneValid");
    Jump_Set = (Jump_Set_t)((int)dlsym(libHandle, "PM_GetEffectiveStance") + 0xEDD);
    PM_NoclipMove = (PM_NoclipMove_t)((int)dlsym(libHandle, "PM_GetEffectiveStance") + 0x1EA7);
    StuckInClient = (StuckInClient_t)dlsym(libHandle, "StuckInClient");
    trap_SendServerCommand = (trap_SendServerCommand_t)dlsym(libHandle, "trap_SendServerCommand");
    Q_strncpyz = (Q_strncpyz_t)dlsym(libHandle, "Q_strncpyz");
    G_AddPredictableEvent = (G_AddPredictableEvent_t)dlsym(libHandle, "G_AddPredictableEvent");
//    G_Say = (G_Say_t)dlsym(libHandle, "G_Say");
    Scr_AddIString = (Scr_AddIString_t)dlsym(libHandle, "Scr_AddIString");

    Scr_ExecThread = (Scr_ExecThread_t)dlsym(libHandle, "Scr_ExecThread");
    BG_GetWeaponIndexForName = (BG_GetWeaponIndexForName_t)dlsym(libHandle, "BG_GetWeaponIndexForName");
    BG_GetInfoForWeapon = (BG_GetInfoForWeapon_t)dlsym(libHandle, "BG_GetInfoForWeapon");
    BG_GetNumWeapons = (BG_GetNumWeapons_t)dlsym(libHandle, "BG_GetNumWeapons");

    Scr_GetEntity = (Scr_GetEntity_t)dlsym(libHandle, "Scr_GetEntity");
    Scr_GetType = (Scr_GetType_t)dlsym(libHandle, "Scr_GetType");
    vectoangles = (vectoangles_t)dlsym(libHandle, "vectoangles");
    ////

    hook_call((int)dlsym(libHandle, "vmMain") + 0xF0, (int)hook_ClientCommand);
//    hook_jmp((int)dlsym(libHandle, "G_Say"), (int)custom_G_Say);


    //Jump
    hook_jmp((int)dlsym(libHandle, "PM_GetEffectiveStance") + 0x16C1, (int)hook_PM_WalkMove_Naked); //UO:sub_24B7C
    resume_addr_PM_WalkMove = (uintptr_t)dlsym(libHandle, "PM_GetEffectiveStance") + 0x18AA;
    hook_jmp((int)dlsym(libHandle, "PM_SlideMove") + 0xB6A, (int)hook_PM_SlideMove_Naked);
    resume_addr_PM_SlideMove = (uintptr_t)dlsym(libHandle, "PM_SlideMove") + 0xBA5;
    hook_jmp((int)dlsym(libHandle, "PM_GetEffectiveStance") + 0xAD, (int)custom_Jump_GetLandFactor);
    hook_jmp((int)dlsym(libHandle, "PM_GetEffectiveStance") + 0x4C, (int)custom_PM_GetReducedFriction);
    hook_call((int)dlsym(libHandle, "PM_GetEffectiveStance") + 0x10B0, (int)hook_Jump_Check);
    hook_call((int)dlsym(libHandle, "ClientEndFrame") + 0x1C4FF, (int)hook_StuckInClient);

    hook_gametype_scripts = new cHook((int)dlsym(libHandle, "GScr_LoadGameTypeScript"), (int)custom_GScr_LoadGameTypeScript);
    hook_gametype_scripts->hook();
    hook_clientendframe = new cHook((int)dlsym(libHandle, "ClientEndFrame"), (int)custom_ClientEndFrame);
    hook_clientendframe->hook();
    hook_PM_FlyMove = new cHook((int)dlsym(libHandle, "PM_GetEffectiveStance") + 0x1354, (int)custom_PM_FlyMove);
    hook_PM_FlyMove->hook();

    hook_ClientBegin = new cHook((int)dlsym(libHandle, "ClientBegin"), (int)custom_ClientBegin);
    hook_ClientBegin->hook();
#if COMPILE_LIBCURL == 1
    hook_G_Say = new cHook((int)dlsym(libHandle, "G_Say"), (int)custom_G_Say);
    hook_G_Say->hook();
#endif
    hook_G_Damage = new cHook((int)dlsym(libHandle, "G_Damage"), (int)custom_G_Damage);
    hook_G_Damage->hook();
//    hook_ClientSpawn = new cHook((int)dlsym(libHandle, "ClientSpawn"), (int)custom_ClientSpawn);
//    hook_ClientSpawn->hook();

    return libHandle;
}

class vcodlib
{
public:
    vcodlib()
    {

        printf("========Initializing VCODLIB========\n");
        // Don't inherit lib of parent
        unsetenv("LD_PRELOAD");

        // Crash handlers for debugging
        signal(SIGSEGV, ServerCrash);
        signal(SIGABRT, ServerCrash);
        
        // Otherwise the printf()'s are printed at crash/end on older os/compiler versions
        setbuf(stdout, NULL);

        printf("> [VCODLIB] Compiled for: CoD: 1.5\n");
        printf("> [VCODLIB] Special thanks to raphael and his libcod1\n");
        printf("> [VCODLIB] Compiled %s %s using GCC %s\n", __DATE__, __TIME__, __VERSION__);


        mprotect((void *)0x08048000, 0x135000, PROT_READ | PROT_WRITE | PROT_EXEC);

        hook_call(0x080894c5, (int)hook_AuthorizeState);
        hook_call(0x0809d8f5, (int)Scr_GetCustomFunction);
        hook_call(0x0809db31, (int)Scr_GetCustomMethod);
    
        //Fastdownload
        hook_jmp(0x8098A34, (int)custom_SV_SendClientMessages);
        hook_jmp(0x808B8A9, (int)custom_SV_WriteDownloadToClient);
        hook_jmp(0x8097A2F, (int)custom_SV_SendMessageToClient);
        hook_jmp(0x808AE44, (int)custom_SV_SendClientGameState);//sv_forcedownload

        //Q3fill fix | Not sure if it has any bugs. I wait till someone reports 
        hook_call(0x0809370B, (int)custom_SV_DirectConnect);

//        hook_jmp(0x0808c7a9, (int)custom_SV_ExecuteClientCommand);

        hook_call(0x08093798, (int)hook_SVC_RemoteCommand);


        hook_com_init = new cHook(0x08070ef8, (int)custom_Com_Init);
        hook_com_init->hook();
        hook_SV_BotUserMove = new cHook(0x080940d2, (int)custom_SV_BotUserMove);
        hook_SV_BotUserMove->hook();
        hook_sys_loaddll = new cHook(0x080d3cdd, (int)custom_Sys_LoadDll);
        hook_sys_loaddll->hook(); 

        hook_sv_begindownload_f = new cHook(0x0808B456, (int)custom_SV_BeginDownload_f);
        hook_sv_begindownload_f->hook();

        hook_SV_MasterHeartbeat = new cHook(0x80922B1, (int)custom_SV_MasterHeartbeat);
		hook_SV_MasterHeartbeat->hook();

        hook_SV_HitchWarning = new cHook(0x080922b1, (int)custom_SV_HitchWarning);
		hook_SV_HitchWarning->hook();

        hook_sv_addoperatorcommands = new cHook(0x808877F, (int)custom_SV_AddOperatorCommands);
        hook_sv_addoperatorcommands->hook();

        hook_cvar_set2 = new cHook(0x08072da8, (int)custom_Cvar_Set2);
        hook_cvar_set2->hook();


        printf("> [PLUGIN LOADED]\n");
    }

    ~vcodlib()
    {
        printf("> [PLUGIN UNLOADED]\n");
    }
};

vcodlib *pro;
void __attribute__ ((constructor)) lib_load(void)
{
    pro = new vcodlib;
}
void __attribute__ ((destructor)) lib_unload(void)
{
    delete pro;
}