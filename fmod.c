#include "inc/fmod.h"

void FMod_init( FMod* fmod ) {
	// Initialisation du système avec 10 cannaux
	FMOD_System_Create( &fmod->sys );
	FMOD_System_Init( fmod->sys, 10, FMOD_INIT_NORMAL, NULL );
}

char FMod_load( FMod* fmod ) {
	// Chargement de tous les sons, si on n'y arrive pas on quitte le sous-programme.
	FMOD_RESULT ret;

	ret = FMOD_System_CreateSound( fmod->sys, "sons/creusement.ogg", FMOD_CREATESAMPLE, 0, &fmod->dig );
	if( ret != FMOD_OK )
		return 0;

	ret = FMOD_System_CreateSound( fmod->sys, "sons/construction.ogg", FMOD_CREATESAMPLE, 0, &fmod->build );
	if( ret != FMOD_OK )
		return 0;

	ret = FMOD_System_CreateSound( fmod->sys, "sons/souffle.ogg", FMOD_CREATESAMPLE, 0, &fmod->blow );
	if( ret != FMOD_OK )
		return 0;

	ret = FMOD_System_CreateSound( fmod->sys, "sons/apparition.ogg", FMOD_CREATESAMPLE, 0, &fmod->start );
	if( ret != FMOD_OK )
		return 0;

	ret = FMOD_System_CreateSound( fmod->sys, "sons/sortie.ogg", FMOD_CREATESAMPLE, 0, &fmod->exit );
	if( ret != FMOD_OK )
		return 0;

	return 1;
}

void FMod_free( FMod* fmod ) {
	// Libération des sons
	FMOD_Sound_Release( fmod->dig );
	FMOD_Sound_Release( fmod->build );
	FMOD_Sound_Release( fmod->blow );
	FMOD_Sound_Release( fmod->start );
	FMOD_Sound_Release( fmod->exit );

	// Libération du système
	FMOD_System_Close( fmod->sys );
	FMOD_System_Release( fmod->sys );
}

void FMod_playSound( FMod* fmod, FMOD_SOUND* sound ) {
	FMOD_System_PlaySound( fmod->sys, FMOD_CHANNEL_FREE, sound, 0, NULL );
}