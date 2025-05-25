#ifndef _GSC_JUMP_HPP_
#define _GSC_JUMP_HPP_

/* gsc functions */
#include "gsc.hpp"

__attribute__ ((naked)) void hook_PM_WalkMove_Naked();
__attribute__ ((naked)) void hook_PM_SlideMove_Naked();
void hook_Jump_Check();
double custom_Jump_GetLandFactor();
double custom_PM_GetReducedFriction();

#endif