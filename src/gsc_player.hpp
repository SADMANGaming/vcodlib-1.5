#ifndef _GSC_PLAYER_HPP_
#define _GSC_PLAYER_HPP_

#include "gsc.hpp"

void gsc_player_button_ads(scr_entref_t ref);
void gsc_player_button_left(scr_entref_t ref);
void gsc_player_button_right(scr_entref_t ref);
void gsc_player_button_forward(scr_entref_t ref);
void gsc_player_button_back(scr_entref_t ref);
void gsc_player_button_jump(scr_entref_t ref);
void gsc_player_button_leanleft(scr_entref_t ref);
void gsc_player_button_leanright(scr_entref_t ref);
void gsc_player_button_reload(scr_entref_t ref);

void gsc_player_processclientcommand(scr_entref_t ref);
void gsc_player_sendservercommand(scr_entref_t ref);

void gsc_player_setvelocity(scr_entref_t ref);
void gsc_player_getvelocity(scr_entref_t ref);

void gsc_player_setgravity(scr_entref_t ref);
void gsc_player_setspeed(scr_entref_t ref);
void gsc_player_getip(scr_entref_t ref);

void gsc_player_connectionlesspackettoclient(scr_entref_t ref);
void gsc_player_getping(scr_entref_t ref);
void gsc_player_dropclient(scr_entref_t ref);
void gsc_player_renamebot(scr_entref_t ref);
void gsc_player_kickbot(scr_entref_t ref);

void gsc_player_getuserinfo(scr_entref_t ref);
void gsc_player_setuserinfo(scr_entref_t ref);
void gsc_player_getangles(scr_entref_t ref);

void gsc_player_kickallbots();
void gsc_player_isonladder(scr_entref_t ref);

#endif