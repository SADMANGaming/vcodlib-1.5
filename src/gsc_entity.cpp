#include "gsc_entity.hpp"


void gsc_entity_setbounds(scr_entref_t ref)
{
    int id = ref.entnum;
    float width, height;

    if (!stackGetParams("ff", &width, &height))
    {
        stackError("gsc_entity_setbounds() one or more arguments is undefined or has a wrong type");
        Scr_AddUndefined();
        return;
    }

    gentity_t *entity = &g_entities[id];

    vec3_t mins = {-height, -width, -width};
    vec3_t maxs = {width, width, height};

    VectorCopy(mins, entity->r.mins);
    VectorCopy(maxs, entity->r.maxs);

    stackPushBool(qtrue);
}