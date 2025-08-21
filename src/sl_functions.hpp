#ifndef _SL_FUNCTIONS_HPP_
#define _SL_FUNCTIONS_HPP_


// vec
typedef void * (*vectoangles_t)(const vec3_t value1, vec3_t angles);
extern vectoangles_t vectoangles;

//va
typedef char* (*va_t)(const char *format, ...);
extern va_t va;

//Client
typedef void (*ClientCommand_t)(int clientNum);
extern ClientCommand_t ClientCommand;

// idk
typedef float (*PitchForYawOnNormal_t)(const float fYaw, const vec3_t normal);
typedef float (*AngleDelta_t)(float angle1, float angle2);
typedef void (*player_die_t)(gentity_s *self, gentity_s *inflictor, gentity_s *attacker, int damage, int meansOfDeath, int iWeapon, const float *vDir, int hitLoc);
typedef void (*SetClientViewAngle_t)(gentity_s *ent, const float *angle);
extern SetClientViewAngle_t SetClientViewAngle;
typedef void (*ClientEndFrame_t)(gentity_s *entity);
typedef void (*ClientThink_real_t)(gentity_s *ent, usercmd_s *ucmd);
typedef void (*StopFollowing_t)(gentity_s *ent);
typedef void (*HudElem_UpdateClient_t)(gclient_s *client, int clientNum, byte which);
typedef vec_t (*VectorNormalize_t)(vec3_t inout);
typedef void (*AddLeanToPosition_t)(float *position, float fViewYaw, float fLeanFrac, float fViewRoll, float fLeanDist);
typedef float (*UnGetLeanFraction_t)(const float fFrac);
typedef float (*AngleNormalize180Accurate_t)(float angle);
typedef float (*AngleNormalize180_t)(float angle);

//// BG
typedef void (*BG_AddPredictableEventToPlayerstate_t)(int newEvent, int eventParm, playerState_t *ps);
typedef int (*BG_CheckProne_t)(
    int passEntityNum, const float *const vPos, float fSize, float fHeight, float fYaw, float * pfTorsoHeight, float * pfTorsoPitch, float * pfWaistPitch, int bAlreadyProne, int bOnGround, float *const vGroundNormal,
    void (*)(trace_t *, const vec3_t, const vec3_t, const vec3_t, const vec3_t, int, int),
    void (*)(trace_t *, const vec3_t, const vec3_t, const vec3_t, const vec3_t, int, int),
    int proneCheckType, float prone_feet_dist);
typedef int (*BG_PlayAnim_t)(playerState_s *ps, int animNum, int bodyPart, int forceDuration, qboolean setTimer, qboolean isContinue, qboolean force);
typedef void (*BG_PlayerStateToEntityState_t)(playerState_s *ps, entityState_s *s, qboolean snap);
typedef int (*BG_CheckProneValid_t)(int passEntityNum, const float *const vPos, float fSize, float fHeight, float fYaw, float *pfTorsoHeight, float *pfTorsoPitch, float *pfWaistPitch, int bAlreadyProne, int bOnGround, float *const vGroundNormal,
    void (*)(trace_t *, const vec3_t, const vec3_t, const vec3_t, const vec3_t, int, int),
    void (*)(trace_t *, const vec3_t, const vec3_t, const vec3_t, const vec3_t, int, int),
    int proneCheckType, float prone_feet_dist);

//typedef WeaponDef_t * (*BG_GetInfoForWeapon_t)(unsigned int weaponIndex);
typedef weaponinfo_t * (*BG_GetInfoForWeapon_t)(unsigned int weaponIndex);
extern BG_GetInfoForWeapon_t BG_GetInfoForWeapon;

typedef int (*BG_GetWeaponIndexForName_t)(const char *name);
extern BG_GetWeaponIndexForName_t BG_GetWeaponIndexForName;

typedef int (*BG_GetNumWeapons_t)(void);
extern BG_GetNumWeapons_t BG_GetNumWeapons;

////

//Cmd
typedef int (*Cmd_FollowCycle_f_t)(gentity_s *ent, int dir);


//// G
typedef void (*G_ClientStopUsingTurret_t)(gentity_s *self);
typedef void (*G_EntUnlink_t)(gentity_s *ent);
typedef void (*G_SetClientContents_t)(gentity_s *ent);
typedef void (*G_SetOrigin_t)(gentity_s *ent, const float *origin);
typedef qboolean (*G_ClientCanSpectateTeam_t)(gclient_s *client, int team);
typedef void (*G_AddPredictableEvent_t)(gentity_t *ent, int event, int eventParm);
extern G_AddPredictableEvent_t G_AddPredictableEvent;

//// PM
typedef bool (*PM_CheckJump_t)();
typedef void (*PM_AirMove_t)();
typedef void (*PM_Friction_t)();
typedef float (*PM_CmdScale_Walk_t)(usercmd_t *cmd);
typedef void (*PM_ClipVelocity_t)(const float *in, const float *normal, float *out, float overbounce);
typedef int (*PM_GetEffectiveStance_t)(playerState_t *ps);
typedef void (*PM_Accelerate_t)(float *wishdir, float wishspeed, float accel);
typedef void (*PM_StepSlideMove_t)(int gravity);
typedef void (*PM_SetMovementDir_t)();
typedef void (*PM_ViewHeightAdjust_t)();
typedef void (*PM_ClearAimDownSightFlag_t)();
typedef int (*PM_GroundSurfaceType_t)();
typedef void (*PM_trace_t)(trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentMask);
typedef void (*PM_AddEvent_t)(int newEvent);
typedef int (*PM_FootstepType_t)(int pm_flags);
typedef float (*PM_GetLandFactor_t)();
typedef void (*PM_NoclipMove_t)();

typedef float (*Jump_Set_t)(float height);
extern Jump_Set_t Jump_Set;

typedef qboolean (*StuckInClient_t)(gentity_s *self);
extern StuckInClient_t StuckInClient;

// Scr
typedef xfunction_t (*Scr_GetFunction_t)(const char** v_functionName, qboolean *v_developer);
extern Scr_GetFunction_t Scr_GetFunction;

typedef xmethod_t (*Scr_GetMethod_t)(const char** v_methodName, qboolean *v_developer);
extern Scr_GetMethod_t Scr_GetMethod;

typedef void (*Scr_Error_t)(const char *string);
extern Scr_Error_t Scr_Error;

typedef int (*Scr_GetNumParam_t)(void);
extern Scr_GetNumParam_t Scr_GetNumParam;

typedef int (*Scr_GetInt_t)(unsigned int param);
extern Scr_GetInt_t Scr_GetInt;

typedef const char* (*Scr_GetString_t)(unsigned int param);
extern Scr_GetString_t Scr_GetString;

typedef void (*Scr_AddBool_t)(qboolean value);
extern Scr_AddBool_t Scr_AddBool;

typedef void (*Scr_AddInt_t)(int value);
extern Scr_AddInt_t Scr_AddInt;

typedef void (*Scr_AddFloat_t)(float value);
extern Scr_AddFloat_t Scr_AddFloat;

typedef void (*Scr_AddString_t)(const char *string);
extern Scr_AddString_t Scr_AddString;

typedef void (*Scr_AddUndefined_t)(void);
extern Scr_AddUndefined_t Scr_AddUndefined;

typedef void (*Scr_AddVector_t)(vec3_t vec);
extern Scr_AddVector_t Scr_AddVector;

typedef void (*Scr_MakeArray_t)(void);
extern Scr_MakeArray_t Scr_MakeArray;

typedef void (*Scr_AddArray_t)(void);
extern Scr_AddArray_t Scr_AddArray;

typedef short (*Scr_ExecEntThread_t)(gentity_t* ent, int callbackHook, unsigned int numArgs);
extern Scr_ExecEntThread_t Scr_ExecEntThread;

typedef short (*Scr_ExecThread_t)(int callbackHook, unsigned int numArgs);
extern Scr_ExecThread_t Scr_ExecThread;

typedef short (*Scr_FreeThread_t)(short thread_id);
extern Scr_FreeThread_t Scr_FreeThread;

typedef unsigned int (*Scr_LoadScript_t)(const char *filename);
typedef int (*Scr_GetFunctionHandle_t)(const char* scriptName, const char* labelName);
typedef int (*Scr_IsSystemActive_t)();
typedef void (*Scr_SetString_t)(uint16_t *to, unsigned int from);

typedef void (*Scr_AddIString_t)(const char* string);
extern Scr_AddIString_t Scr_AddIString;

typedef int (*Scr_GetType_t)(unsigned int param);
extern Scr_GetType_t Scr_GetType;

typedef gentity_t * (*Scr_GetEntity_t)(unsigned int index);
extern Scr_GetEntity_t Scr_GetEntity;

// G
//typedef void (*G_Say_t)(gentity_s *ent, gentity_s *target, int mode, const char *chatText);
//extern G_Say_t G_Say;


// Trap
typedef void (*trap_Argv_t)(int arg, char *buffer, int bufferLength);
typedef void (*trap_UnlinkEntity_t)(gentity_t *gEnt);
typedef void (*trap_GetUsercmd_t)(int clientNum, usercmd_s *cmd);
typedef int (*trap_GetArchivedClientInfo_t)(int clientNum, int archiveTime, playerState_t *ps, clientState_t *cs);
typedef void (*trap_SendServerCommand_t)(int clientnum, svscmd_type type, const char *text);
extern trap_SendServerCommand_t trap_SendServerCommand;


#endif