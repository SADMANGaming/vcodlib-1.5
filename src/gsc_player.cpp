#include "gsc_player.hpp"

void gsc_player_processclientcommand(scr_entref_t ref)
{
    int id = ref.entnum;

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_player_processclientcommand() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    ClientCommand(id);

    stackPushBool(qtrue);
}


void gsc_player_setvelocity(scr_entref_t ref)
{
    int id = ref.entnum;
    vec3_t velocity;

    if ( !stackGetParams("v", &velocity) )
    {
        stackError("gsc_player_setvelocity() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if ( id >= MAX_CLIENTS )
    {
        stackError("gsc_player_setvelocity() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    playerState_t *ps = SV_GameClientNum(id);
    VectorCopy(velocity, ps->velocity);

    stackPushBool(qtrue);
}

void gsc_player_getvelocity(scr_entref_t ref)
{
    int id = ref.entnum;

    if ( id >= MAX_CLIENTS )
    {
        stackError("gsc_player_getvelocity() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    playerState_t *ps = SV_GameClientNum(id);

    stackPushVector(ps->velocity);
}

void gsc_player_connectionlesspackettoclient(scr_entref_t ref)
{
    int id = ref.entnum;
    char *cmd;

    if ( !stackGetParams("s", &cmd) )
    {
        stackError("gsc_player_connectionlesspackettoclient() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if ( id >= MAX_CLIENTS )
    {
        stackError("gsc_player_connectionlesspackettoclient() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];
    NET_OutOfBandPrint(NS_SERVER, client->netchan.remoteAddress, cmd);

    stackPushBool(qtrue);
}

void gsc_player_getping(scr_entref_t ref)
{
    int id = ref.entnum;

    if ( id >= MAX_CLIENTS )
    {
        stackError("gsc_player_getping() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];
    stackPushInt(client->ping);
}

void gsc_player_dropclient(scr_entref_t ref)
{
    int id = ref.entnum;
    char *reason;

    if ( Scr_GetNumParam() > 0 && !stackGetParams("s", &reason) )
    {
        stackError("gsc_player_dropclient() argument has a wrong type");
        stackPushUndefined();
        return;
    }

    if ( id >= MAX_CLIENTS )
    {
        stackError("gsc_player_dropclient() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    if(Scr_GetNumParam() > 0)
        SV_DropClient(client, reason);
    else
        SV_DropClient(client, NULL);

    stackPushBool(qtrue);
}

void gsc_player_button_ads(scr_entref_t ref)
{
    int id = ref.entnum;

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_player_button_ads() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    stackPushBool(client->lastUsercmd.buttons & BUTTON_ZOOM ? qtrue : qfalse);
}

void gsc_player_button_left(scr_entref_t ref)
{
    int id = ref.entnum;

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_player_button_left() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    stackPushBool(client->lastUsercmd.rightmove == KEY_LEFT ? qtrue : qfalse);
}

void gsc_player_button_right(scr_entref_t ref)
{
    int id = ref.entnum;

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_player_button_right() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    stackPushBool(client->lastUsercmd.rightmove == KEY_RIGHT ? qtrue : qfalse);
}

void gsc_player_button_forward(scr_entref_t ref)
{
    int id = ref.entnum;

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_player_button_forward() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    stackPushBool(client->lastUsercmd.forwardmove == KEY_FORWARD ? qtrue : qfalse);
}

void gsc_player_button_back(scr_entref_t ref)
{
    int id = ref.entnum;

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_player_button_back() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    stackPushBool(client->lastUsercmd.forwardmove == KEY_BACK ? qtrue : qfalse);
}

void gsc_player_button_jump(scr_entref_t ref)
{
    int id = ref.entnum;

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_player_button_jump() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    stackPushBool(client->lastUsercmd.upmove == KEY_JUMP ? qtrue : qfalse);
}

void gsc_player_button_leanleft(scr_entref_t ref)
{
    int id = ref.entnum;

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_player_button_leanleft() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];
    
    stackPushBool(client->lastUsercmd.wbuttons & WBUTTON_LEANLEFT ? qtrue : qfalse);
}

void gsc_player_button_leanright(scr_entref_t ref)
{
    int id = ref.entnum;

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_player_button_leanright() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    stackPushBool(client->lastUsercmd.wbuttons & WBUTTON_LEANRIGHT ? qtrue : qfalse);
}

void gsc_player_button_reload(scr_entref_t ref)
{
    int id = ref.entnum;

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_player_button_reload() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    stackPushBool(client->lastUsercmd.wbuttons & WBUTTON_RELOAD ? qtrue : qfalse);
}


void gsc_player_sendservercommand(scr_entref_t ref)
{
    int id = ref.entnum;
    char *message;

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_player_sendservercommand() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }
    //client_t *cl, int type, const char *fmt, ...
    
    if ( !stackGetParams("s", &message) )
    {
        stackError("gsc_player_sendservercommand() one or more arguments is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }
    
    client_t *client = &svs.clients[id];

    SV_SendServerCommand(client, 1, message);
    stackPushBool(qtrue);
}

extern customPlayerState_t customPlayerState[MAX_CLIENTS];
void gsc_player_setspeed(scr_entref_t ref)
{
    int id = ref.entnum;
    int speed;

	if ( !stackGetParams("i", &speed) )
	{
		stackError("gsc_player_setspeed() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setspeed() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	if ( speed < 0 )
	{
		stackError("gsc_player_setspeed() param must be equal or above zero");
		stackPushUndefined();
		return;
	}

	customPlayerState[id].speed = speed;

	stackPushBool(qtrue);
}
void gsc_player_setgravity(scr_entref_t ref)
{
	int id = ref.entnum;
    int gravity;

	if ( !stackGetParams("i", &gravity) )
	{
		stackError("gsc_player_setgravity() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( id >= MAX_CLIENTS )
	{
		stackError("gsc_player_setgravity() entity %i is not a player", id);
		stackPushUndefined();
		return;
	}

	if ( gravity < 0 )
	{
		stackError("gsc_player_setgravity() param must be equal or above zero");
		stackPushUndefined();
		return;
	}

	customPlayerState[id].gravity = gravity;

	stackPushBool(qtrue);
}

void gsc_player_getip(scr_entref_t ref)
{
    int id = ref.entnum;

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_player_getip() entity %i is not a player", id);
        Scr_AddUndefined();
        return;
    }

    client_t *client = &svs.clients[id];
    char ip[16];
    
    snprintf(ip, sizeof(ip), "%d.%d.%d.%d",
        client->netchan.remoteAddress.ip[0],
        client->netchan.remoteAddress.ip[1],
        client->netchan.remoteAddress.ip[2],
        client->netchan.remoteAddress.ip[3]);

    Scr_AddString(ip);
}

void gsc_player_renamebot(scr_entref_t ref) {
    int id = ref.entnum;
	const char* key = Scr_GetString(0);
	char userinfo[MAX_STRING_CHARS];
	getuserinfo(id, userinfo, sizeof(userinfo));
	
	Info_SetValueForKey(userinfo, "name", key);
	setuserinfo(id, userinfo);
	
	client_t* cl = &svs.clients[id];
	if(cl) {
		memcpy(&cl->name, key, 32);
		cl->name[31] = '\0';
	}
}

void gsc_player_kickbot(scr_entref_t ref) { //weird playercmd > bot
    int id = ref.entnum;
	client_t* cl = &svs.clients[id];
	if(cl) {
		SV_DropClient(cl, "");
		cl->state = CS_FREE;
	}
}