#include "gsc_weapon.hpp"

qboolean isValidWeaponId(int id)
{
    int weps = BG_GetNumWeapons();

    if (id > weps || id <= 0 || weps == 0)
        return qfalse;

    return qtrue;
}

void gsc_weapons_setweaponcookable()
{
    int id;
    char *name;
    int cookable;

    if (stackGetParams("si", &name, &cookable)) 
    {
        id = BG_GetWeaponIndexForName(name);
    }
    else if (!stackGetParams("ii", &id, &cookable))
    {
        stackError("gsc_weapons_setweaponcookable() one or more arguments is undefined or has a wrong type");
        Scr_AddUndefined();
        return;
    }

    if (!isValidWeaponId(id))
    {
        Scr_AddUndefined();
        return;
    }

    weaponinfo_t *weapon = BG_GetInfoForWeapon(id);
    weapon->cookOffHold = cookable;

    Scr_AddBool(qtrue);
}

void gsc_weapons_setweaponfusetime()
{
    int id;
    char *name;
    int time;

    if (stackGetParams("si", &name, &time)) 
    {
        id = BG_GetWeaponIndexForName(name);
    }
    else if (!stackGetParams("ii", &id, &time))
    {
        stackError("gsc_weapons_setweaponfusetime() one or more arguments is undefined or has a wrong type");
        Scr_AddUndefined();
        return;
    }

    if (!isValidWeaponId(id))
    {
        Scr_AddUndefined();
        return;
    }

    weaponinfo_t *weapon = BG_GetInfoForWeapon(id);
    weapon->fuseTime = time;

    Scr_AddBool(qtrue);
}

void gsc_weapon_getfullclipammo()
{
    char *weaponName;

    if (!stackGetParams("s", &weaponName))
    {
        stackError("gsc_weapon_getfullclipammo() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    int id = BG_GetWeaponIndexForName(weaponName);
    if (!isValidWeaponId(id))
    {
        stackPushUndefined();
        return;
    }

    weaponinfo_t *weapon = BG_GetInfoForWeapon(id);
    if (!weapon)
    {
        stackPushUndefined();
        return;
    }
    // ts tuff man
    // clipsize = weapon + 0x1B0 / 4 ( 4 = char* size)
    // so clipsize offset is 108
    int clipSize = *(int *)((char *)weapon + 0x1B0);

    stackPushInt(clipSize);
}
