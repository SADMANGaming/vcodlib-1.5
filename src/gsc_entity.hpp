#ifndef _GSC_ENTITY_HPP_
#define _GSC_ENTITY_HPP_

#include "gsc.hpp"

void gsc_entity_setbounds(scr_entref_t ref);
void gsc_entity_setclipmask(scr_entref_t ref);
void gsc_entity_getclipmask(scr_entref_t ref);
void gsc_entity_showtoplayer(scr_entref_t ref);

#endif