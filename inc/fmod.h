#ifndef _ECE_SPRITS_FMOD_H_
#define _ECE_SPRITS_FMOD_H_

#include "structures.h"

void FMod_init( FMod* fmod );
char FMod_load( FMod* fmod );
void FMod_free( FMod* fmod );
void FMod_playSound( FMod* fmod, FMOD_SOUND* sound );

#endif
