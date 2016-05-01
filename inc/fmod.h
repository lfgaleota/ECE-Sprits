#ifndef _ECE_SPRITS_FMOD_H_
#define _ECE_SPRITS_FMOD_H_

#include "structures.h"
#include <stdio.h>

void FMod_init( FMod* fmod );
char FMod_load( FMod* fmod );
void FMod_free( FMod* fmod );
void FMod_playSound( FMod* fmod, FMOD_SOUND* sound );

char FMod_loadLevelMusic( FMod* fmod, char* path );
void FMod_playMusic( FMod* fmod, FMOD_SOUND* music );
void FMod_stopMusic( FMod* fmod );
void FMod_pauseMusic( FMod* fmod );

#endif
