#ifndef _BIN_FUNCTIONS_HPP_
#define _BIN_FUNCTIONS_HPP_

// Cvar
typedef cvar_t* (*Cvar_Get_t)(const char *var_name, const char *var_value, unsigned short flags);
static const Cvar_Get_t Cvar_Get = (Cvar_Get_t)0x08072a7c;

typedef cvar_t* (*Cvar_FindVar_t)(const char *var_name);
static const Cvar_FindVar_t Cvar_FindVar = (Cvar_FindVar_t)0x08072916;

// Cmd
typedef char* (*Cmd_Argv_t)(int arg);
static const Cmd_Argv_t Cmd_Argv = (Cmd_Argv_t)0x080600f4;

typedef int (*Cmd_Argc_t)();
static const Cmd_Argc_t Cmd_Argc = (Cmd_Argc_t)0x080600ea;

typedef int (*Cmd_FollowCycle_f_t)(gentity_s *ent, int dir);

typedef void (*Cmd_TokenizeString_t)(const char *text_in);
static const Cmd_TokenizeString_t Cmd_TokenizeString = (Cmd_TokenizeString_t)0x08060423;

// Com
typedef void (*Com_DPrintf_t)(const char *format, ...);
static const Com_DPrintf_t Com_DPrintf = (Com_DPrintf_t)0x0806fc5f;

typedef void (*Com_PrintMessage_t)(int channel, const char *message);
static const Com_PrintMessage_t Com_PrintMessage = (Com_PrintMessage_t)0x0806f827;

typedef void (*Com_Printf_t)(const char *format, ...);
static const Com_Printf_t Com_Printf = (Com_Printf_t)0x0806fc10;

typedef void (*Com_Error_t)(errorParm_t code, const char *format, ...);
static const Com_Error_t Com_Error = (Com_Error_t)0x0806fe74;


//// PB
typedef const char * (*PBAuthClient_t)(const char *clientAddress, int cl_punkbuster, const char *PBGuid);
static const PBAuthClient_t PBAuthClient = (PBAuthClient_t)0x080c191c;

typedef void (*PbSvAddEvent_t)(int event, int clientNum, int offset, byte *msg);
static const PbSvAddEvent_t PbSvAddEvent = (PbSvAddEvent_t)0x080C174C;

typedef void (*PbPassConnectString_t)(const char *addr, byte *data);
static const PbPassConnectString_t PbPassConnectString = (PbPassConnectString_t)0x080C18EC;


// SL
typedef char* (*SL_ConvertToString_t)(unsigned int index);
static const SL_ConvertToString_t SL_ConvertToString = (SL_ConvertToString_t)0x080a3460; //done

// Scr
typedef unsigned short (*Scr_AllocArray_t)(void);
static const Scr_AllocArray_t Scr_AllocArray = (Scr_AllocArray_t)0x080a5ab4;

// SV
typedef playerState_t* (*SV_GameClientNum_t)(int num);
static const SV_GameClientNum_t SV_GameClientNum = (SV_GameClientNum_t)0x0808d331;

typedef gentity_t* (*SV_ClientThink_t)(client_t *cl, usercmd_t *cmd);
static const SV_ClientThink_t SV_ClientThink = (SV_ClientThink_t)0x0808ca58;

typedef void (*SV_FreeClient_t)(client_t *client);
static const SV_FreeClient_t SV_FreeClient = (SV_FreeClient_t)0x08089daf;

typedef gentity_t * (*SV_GentityNum_t)(int num);
static const SV_GentityNum_t SV_GentityNum = (SV_GentityNum_t)0x0808d314;

typedef void (*SV_UserinfoChanged_t)(client_t *client);
static const SV_UserinfoChanged_t SV_UserinfoChanged = (SV_UserinfoChanged_t)0x0808c4fd;

typedef void (*SV_DropClient_t)(client_t *drop, const char *reason);
static const SV_DropClient_t SV_DropClient = (SV_DropClient_t)0x0808ac11;


// SVC
typedef void (*SVC_Status_t)(netadr_t from);
static const SVC_Status_t SVC_Status = (SVC_Status_t)0x0809246E;

typedef void (*SVC_Info_t)(netadr_t from);
static const SVC_Info_t SVC_Info = (SVC_Info_t)0x08092A74;

typedef void (*SVC_RemoteCommand_t)(netadr_t from, msg_t *msg);
static const SVC_RemoteCommand_t SVC_RemoteCommand = (SVC_RemoteCommand_t)0x080930D0;


// NET
typedef const char * (*NET_AdrToString_t)(netadr_t a);
static const NET_AdrToString_t NET_AdrToString = (NET_AdrToString_t)0x08083e10;

typedef int (*NET_CompareBaseAdr_t)(netadr_t a,netadr_t b);
static const NET_CompareBaseAdr_t NET_CompareBaseAdr = (NET_CompareBaseAdr_t)0x08083de3;

typedef qboolean (*NET_IsLocalAddress_t)(netadr_t from);
static const NET_IsLocalAddress_t NET_IsLocalAddress = (NET_IsLocalAddress_t)0x0808403c;

typedef void (*NET_OutOfBandPrint_t)(netsrc_t net_socket, netadr_t adr, const char *format, ...);
static const NET_OutOfBandPrint_t NET_OutOfBandPrint = (NET_OutOfBandPrint_t)0x0808428e;

typedef qboolean (*NET_CompareAdr_t)(netadr_t a, netadr_t b);
static const NET_CompareAdr_t NET_CompareAdr = (NET_CompareAdr_t)0x0808400f;

typedef void (*NetadrToSockadr_t)(netadr_t *a, struct sockaddr_in *s);
static const NetadrToSockadr_t NetadrToSockadr = (NetadrToSockadr_t)0x080d4a0c;

typedef qboolean (*NET_StringToAdr_t)(const char *s, netadr_t *a);
static const NET_StringToAdr_t NET_StringToAdr = (NET_StringToAdr_t)0x080844e0;

#endif