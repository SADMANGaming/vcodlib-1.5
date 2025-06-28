#ifndef _BIN_FUNCTIONS_HPP_
#define _BIN_FUNCTIONS_HPP_

// Get
//xtnded compatibility
typedef void (*getuserinfo_t)( int index, char *buffer, int bufferSize );
static const getuserinfo_t getuserinfo = (getuserinfo_t)0x80903B5;

typedef void (*setuserinfo_t)( int,const char*);
static const setuserinfo_t setuserinfo = (setuserinfo_t)0x809030B;
//
///

// Cvar
typedef cvar_t* (*Cvar_Get_t)(const char *var_name, const char *var_value, unsigned short flags);
static const Cvar_Get_t Cvar_Get = (Cvar_Get_t)0x08072a7c;

typedef cvar_t* (*Cvar_FindVar_t)(const char *var_name);
static const Cvar_FindVar_t Cvar_FindVar = (Cvar_FindVar_t)0x08072916;

typedef cvar_t* (*Cvar_Set_t)(const char *var_name, const char *value);
static const Cvar_Set_t Cvar_Set = (Cvar_Set_t)0x8073100;

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

typedef int (*Com_sprintf_t)(char *dest, int size, const char *format, ...);
static const Com_sprintf_t Com_sprintf = (Com_sprintf_t)0x80861A6; //tbd

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

typedef void (*SV_DirectConnect_t)(netadr_t from);
static const SV_DirectConnect_t SV_DirectConnect = (SV_DirectConnect_t)0x8089E7E;

typedef void (QDECL *SV_SendServerCommand_t)(client_t *cl, int type, const char *fmt, ...);
static const SV_SendServerCommand_t SV_SendServerCommand = (SV_SendServerCommand_t)0x80920DE;

typedef void (*SV_Netchan_TransmitNextFragment_t)(netchan_t *chan);
static const SV_Netchan_TransmitNextFragment_t SV_Netchan_TransmitNextFragment = (SV_Netchan_TransmitNextFragment_t)0x80948D0;

typedef qboolean (*SV_Netchan_Transmit_t)(client_t *client, byte *data, int length);
static const SV_Netchan_Transmit_t SV_Netchan_Transmit = (SV_Netchan_Transmit_t)0x80948E3;

typedef void (*SV_SendClientSnapshot_t)(client_t *cl);
static const SV_SendClientSnapshot_t SV_SendClientSnapshot = (SV_SendClientSnapshot_t)0x8097C2F;

typedef void (*SV_WWWRedirectClient_t)( client_t *cl, msg_t *msg );
static const SV_WWWRedirectClient_t SV_WWWRedirectClient = (SV_WWWRedirectClient_t)0x808B6FC;

typedef void (*SV_SendMessageToClient_t)(msg_t *msg, client_t *cl);
static const SV_SendMessageToClient_t SV_SendMessageToClient = (SV_SendMessageToClient_t)0x8097A2F;

typedef void (*SV_UpdateServerCommandsToClient_t)(client_t *client, msg_t *msg);
static const SV_UpdateServerCommandsToClient_t SV_UpdateServerCommandsToClient = (SV_UpdateServerCommandsToClient_t)0x80959D0;//tbd

// SVC
typedef void (*SVC_Status_t)(netadr_t from);
static const SVC_Status_t SVC_Status = (SVC_Status_t)0x0809246E;

typedef void (*SVC_Info_t)(netadr_t from);
static const SVC_Info_t SVC_Info = (SVC_Info_t)0x08092A74;

typedef void (*SVC_RemoteCommand_t)(netadr_t from, msg_t *msg);
static const SVC_RemoteCommand_t SVC_RemoteCommand = (SVC_RemoteCommand_t)0x080930D0;

//Info
typedef char* (*Info_ValueForKey_t)(const char *s, const char *key);
static const Info_ValueForKey_t Info_ValueForKey = (Info_ValueForKey_t)0x8086397;

typedef void (*Info_SetValueForKey_t)(char *s, const char *key, const char *value);
static const Info_SetValueForKey_t Info_SetValueForKey = (Info_SetValueForKey_t)0x8086855;

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

typedef const char* (*NET_idkPrint_t)(const char *format, ...);
static const NET_idkPrint_t NET_idkPrint = (NET_idkPrint_t)0x806FC10;

//Cmd
typedef void (*Cmd_AddCommand_t)(const char *cmd_name, xcommand_t function);
static const Cmd_AddCommand_t Cmd_AddCommand = (Cmd_AddCommand_t)0x806043E; //txt str

//MSG
typedef int (*MSG_WriteBitsCompress_t)(const byte *datasrc, byte *buffdest, int bytecount);
static const MSG_WriteBitsCompress_t MSG_WriteBitsCompress = (MSG_WriteBitsCompress_t)0x807FC1B; 

typedef void (*MSG_WriteString_t)(msg_t *msg, const char *s);
static const MSG_WriteString_t MSG_WriteString = (MSG_WriteString_t)0x807FE03;

typedef void (*MSG_WriteLong_t)(msg_t *msg, int c);
static const MSG_WriteLong_t MSG_WriteLong = (MSG_WriteLong_t)0x807FDB5;

typedef void (*MSG_WriteByte_t)(msg_t *msg, int c);
static const MSG_WriteByte_t MSG_WriteByte = (MSG_WriteByte_t)0x807FCD8;

typedef void (*MSG_WriteShort_t)(msg_t *msg, int c);
static const MSG_WriteShort_t MSG_WriteShort = (MSG_WriteShort_t)0x807FD65;

typedef void (*MSG_WriteData_t)(msg_t *msg, const void *data, int length);
static const MSG_WriteData_t MSG_WriteData = (MSG_WriteData_t)0x807FD10;

typedef void (*MSG_WriteBigString_t)(msg_t *msg, const char *s);
static const MSG_WriteBigString_t MSG_WriteBigString = (MSG_WriteBigString_t)0x807FEDB;

typedef void (*MSG_Init_t)(msg_t *buf, byte *data, int length);
static const MSG_Init_t MSG_Init = (MSG_Init_t)0x807F928;

//FS
typedef int (*FS_idPak_t)(char *pak, const char *base); //FS_idPak
static const FS_idPak_t FS_idPak = (FS_idPak_t)0x80754EA;

typedef long (*FS_SV_FOpenFileRead_t)(const char *filename, fileHandle_t *fp);
static const FS_SV_FOpenFileRead_t FS_SV_FOpenFileRead = (FS_SV_FOpenFileRead_t)0x8074596;

//Q
typedef void (*Q_strncpyz_t)(char *dest, const char *src, int destsize);

//Z
typedef void* (*Z_MallocInternal_t)(int size);
static const Z_MallocInternal_t Z_MallocInternal = (Z_MallocInternal_t)0x806BBCC;

//FS
typedef int (*FS_Read_t)(void *buffer, int len, fileHandle_t f);
static const FS_Read_t FS_Read = (FS_Read_t)0x8062960;

//SYS
typedef qboolean (*Sys_IsLANAddress_t)(netadr_t adr);
static const Sys_IsLANAddress_t Sys_IsLANAddress = (Sys_IsLANAddress_t)0x80D4EBC;

#endif