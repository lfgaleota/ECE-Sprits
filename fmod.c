#include "inc/fmod.h"

void FMod_init( FMod* fmod ) {
	// Initialisation du système avec 10 cannaux
	FMOD_System_Create( &fmod->sys );
	FMOD_System_Init( fmod->sys, 10, FMOD_INIT_NORMAL, NULL );
	FMOD_System_GetMasterChannelGroup( fmod->sys, &fmod->channel_master );
	FMOD_System_GetChannel( fmod->sys, 1, &fmod->channel_1 );
	FMOD_System_GetChannel( fmod->sys, 2, &fmod->channel_2 );
	//FMOD_ChannelGroup_SetVolume( fmod->channel_master, 0 );
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

	ret = FMOD_System_CreateSound( fmod->sys, "sons/menu.ogg", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &fmod->menu );
	if( ret != FMOD_OK )
		return 0;

	fmod->music = NULL;

	return 1;
}

char FMod_loadLevelMusic( FMod* fmod, char* path ) {
	FMOD_RESULT ret;
	char fullpath[ BUFFER_SIZE ];

	if( fmod->music != NULL ) {
		FMOD_Sound_Release( fmod->music );
		fmod->music = NULL;
	}

	sprintf( fullpath, "%s/musique.ogg", path );

	ret = FMOD_System_CreateSound( fmod->sys, fullpath, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &fmod->music );
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

void FMod_playSound( FMod* fmod, FMOD_SOUND* sound ) {int index;
	if( FMOD_Channel_GetIndex( fmod->channel_2, &index ) ) {
		index = FMOD_CHANNEL_FREE;
	}

	FMOD_System_PlaySound( fmod->sys, index, sound, 0, NULL );
}

void FMod_playMusic( FMod* fmod, FMOD_SOUND* music ) {
	int index;
	if( FMOD_Channel_GetIndex( fmod->channel_1, &index ) ) {
		index = FMOD_CHANNEL_FREE;
	}

	FMOD_Sound_SetLoopCount( music, -1 );
	FMOD_System_PlaySound( fmod->sys, index, music, 0, NULL );
}

void FMod_pauseMusic( FMod* fmod ) {
	FMOD_BOOL etat;

	FMOD_Channel_GetPaused( fmod->channel_1, &etat);

	if (etat == 1) // Si la chanson est en pause
		FMOD_Channel_SetPaused( fmod->channel_1, 0 ); // On enlève la pause
	else // Sinon, elle est en cours de lecture
		FMOD_Channel_SetPaused( fmod->channel_1, 1 ); // On met en pause
}

void FMod_stopMusic( FMod* fmod ) {
	FMOD_Channel_Stop( fmod->channel_1 );
}