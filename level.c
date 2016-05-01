#include <stdlib.h>
#include "inc/level.h"

BITMAP** Level_loadBitmaps( char* path, unsigned short count ) {
	int i;
	BITMAP** bmps = malloc( sizeof( BITMAP* ) * count );
	char* full_path;

	if( bmps ) {
		full_path = malloc( sizeof( char ) * ( strlen( path ) + 10 ) );

		if( full_path ) {
			for( i = 0; i < count; i++ ) {
				sprintf( full_path, "%s%d.png", path, i + 1 );

				bmps[ i ] = load_png( full_path, NULL );
				if( bmps[ i ] == NULL ) {
					allegro_message( "Erreur chargement %s", full_path );
					return 0;
				}
			}

			free( full_path );

			return bmps;
		}
	}

	return NULL;
}

Frames Level_loadFrames( char* path, unsigned short count ) {
	Frames frames;

	frames.bmps = Level_loadBitmaps( path, count );
	frames.count = count;

	return frames;
}

void Level_freeBitmap( BITMAP* bmp ) {
	if( bmp )
		destroy_bitmap( bmp );
}

void Level_freeBitmaps( BITMAP** bmps, unsigned short count ) {
	int i;
	
	if( !bmps )
		return;
	
	for( i = 0; i < count; i++ ) {
		Level_freeBitmap( bmps[ i ] );
	}
	
	free( bmps );
}

void Level_freeFrames( Frames* frames ) {
	Level_freeBitmaps( frames->bmps, frames->count );
}

char Level_loadImages( char* path, Level* level ) {
	char fullpath[ BUFFER_SIZE ];

	sprintf( fullpath, "%s/collision.png", path );
	level->bmps.col = load_png( fullpath, NULL );
	if( !level->bmps.col ) {
		allegro_message( "Erreur chargement %s", fullpath );
		return 0;
	}

	sprintf( fullpath, "%s/background.jpg", path );
	level->bmps.back = load_jpg( fullpath, NULL );
	if( !level->bmps.back ) {
		allegro_message( "Erreur chargement %s", fullpath );
		return 0;
	}

	sprintf( fullpath, "%s/foreground.png", path );
	level->bmps.fore = load_png( fullpath, NULL );
	if( !level->bmps.fore ) {
		allegro_message( "Erreur chargement %s", fullpath );
		return 0;
	}

	level->bmps.stickmen_walking = Level_loadFrames( "images/sticks/stickMan", 20 );
	level->bmps.stickmen_falling = Level_loadFrames( "images/sticks/stickManChute", 1 );
	level->bmps.stickmen_dying = Level_loadFrames( "images/sticks/stickManMort", 5 );
	level->bmps.stickmen_falldying = Level_loadFrames( "images/sticks/stickManMortChute", 5 );
	level->bmps.stickmen_starting = Level_loadFrames( "images/sticks/stickManDepart", 11 );
	level->bmps.stickmen_exiting = Level_loadFrames( "images/sticks/stickManArrive", 10 );
	level->bmps.stickmen_digging = Level_loadFrames( "images/sticks/stickManCreuse", 3 );
	level->bmps.stickmen_building = Level_loadFrames( "images/sticks/stickManConstruit", 3 );
	level->bmps.stickmen_blowing = Level_loadFrames( "images/sticks/stickManSouffle", 1 );
	level->bmps.start = Level_loadFrames( "images/portails/portailDebut", 1 );
	level->bmps.exit = Level_loadFrames( "images/portails/portailArrivee", 1 );

	sprintf( fullpath, "%s/texture_wall.jpg", path );
	level->bmps.wall = load_jpg( fullpath, NULL );
	sprintf( fullpath, "%s/texture_deathzone.jpg", path );
	level->bmps.deathzone = load_jpg( fullpath, NULL );

	level->bmps.arrow = Level_loadBitmaps( "images/arrows/arrow", 9 );

	level->bmps.capacity_build = load_png( "images/capacites/construire.png", NULL );
	level->bmps.capacity_dig = load_png( "images/capacites/creuser.png", NULL );
	level->bmps.capacity_blow = load_png( "images/capacites/souffler.png", NULL );

	level->bmps.branch = load_png( "images/capacites/branche.png", NULL );
	level->bmps.stick_fire = Level_loadBitmaps( "images/capacites/flamme", 3 );
	level->bmps.blow = load_png( "images/capacites/souffle.png", NULL );

	level->bmps.ui_accelerate = load_png( "images/acc.png", NULL );
	level->bmps.ui_pause = load_png( "images/pause.png", NULL );
	level->bmps.ui_menu = load_png( "images/menu.png", NULL );

	if( !level->bmps.stickmen_walking.bmps || !level->bmps.stickmen_falling.bmps || !level->bmps.stickmen_dying.bmps || !level->bmps.stickmen_falldying.bmps || !level->bmps.stickmen_starting.bmps || !level->bmps.stickmen_exiting.bmps || !level->bmps.stickmen_digging.bmps || !level->bmps.stickmen_building.bmps || !level->bmps.stickmen_blowing.bmps || !level->bmps.start.bmps || !level->bmps.exit.bmps || !level->bmps.arrow || !level->bmps.capacity_build || !level->bmps.capacity_dig || !level->bmps.capacity_blow || !level->bmps.branch || !level->bmps.stick_fire || !level->bmps.blow || !level->bmps.ui_accelerate || !level->bmps.ui_pause || !level->bmps.ui_menu ) {
		allegro_message( "Impossible de charger les images" );
		return 0;
	}

	level->bmps.droidsans_14_mono = load_font( "polices/droidsans_14_mono.pcx", NULL, NULL );

	return 1;
}

void Level_parseConfigurationLine( Level* level, char* line, int* lines_parsed ) {
	char key[ BUFFER_SIZE ], value[ BUFFER_SIZE ];

	if( !level || !line )
		return;

	if( Config_extractKeyValue( line, key, value ) ) {
		if( strcmp( key, "nb_stickmen_max" ) == 0 ) {
			level->nb_stickmen_max = (unsigned short) strtoul( value, 0, 10 );
			(*lines_parsed)++;
		} else if( strcmp( key, "nb_stickmen_should_arrive" ) == 0 ) {
			level->nb_stickmen_should_arrive = (unsigned short) strtoul( value, 0, 10 );
			(*lines_parsed)++;
		} else if( strcmp( key, "counter_stickmen_arrival_max" ) == 0 ) {
			level->counter_stickmen_arrival_max = (unsigned short) strtoul( value, 0, 10 );
			(*lines_parsed)++;
		} else if( strcmp( key, "slow_dt" ) == 0 ) {
			level->slow_dt = strtof( value, 0 );
			(*lines_parsed)++;
		} else if( strcmp( key, "fast_dt" ) == 0 ) {
			level->fast_dt = strtof( value, 0 );
			(*lines_parsed)++;
		} else if( strcmp( key, "start" ) == 0 ) {
			if( Config_parseVector2( &level->start, value ) )
				(*lines_parsed)++;
		} else if( strcmp( key, "exit" ) == 0 ) {
			if( Config_parseVector2( &level->exit, value ) )
				(*lines_parsed)++;
		} else if( strcmp( key, "gravity" ) == 0 ) {
			if( Config_parseVector2( &level->gravity, value ) )
				(*lines_parsed)++;
		} else if( strcmp( key, "movement" ) == 0 ) {
			if( Config_parseVector2( &level->movement, value ) )
				(*lines_parsed)++;
		} else if( strcmp( key, "capacities" ) == 0 ) {
			if( Config_parseCapacitiesNumber( &level->capacities, value ) )
				(*lines_parsed)++;
		}
	}
}

char Level_loadProperties( char* path, Level* level ) {
	FILE* properties;
	int lines_parsed = 0;
	char buffer[ BUFFER_SIZE ];
	sprintf( buffer, "%s/level", path );

	properties = fopen( buffer, "r" );

	if( !properties ) {
		allegro_message( "Impossible de charger le fichier de configuration du niveau." );
		return 0;
	}

	while( fgets( buffer, BUFFER_SIZE, properties ) != NULL )  {
		Level_parseConfigurationLine( level, buffer, &lines_parsed );
	}

	if( lines_parsed == 10 ) {
		strcpy( level->path, path );
		return 1;
	}

	allegro_message( "Fichier de configuration du niveau invalide." );
	return 0;
}

Level* Level_load( char* path ) {
	Level* level = malloc( sizeof( Level ) );
	
	if( !level ) {
		allegro_message( "Allocation du niveau échouée!" );
		return NULL;
	}

	if( !Level_loadProperties( path, level ) ) {
		free( level );
		return NULL;
	}

	if( !Level_loadImages( path, level ) ) {
		free( level );
		return NULL;
	}
	
	return level;
}

Level* Level_loadStandard( int number ) {
	char fullpath[] = "niveauXXX/";
	sprintf( fullpath, "niveau%d/", number );

	return Level_load( fullpath );
}

void Level_free( Level* level ) {
	if( level ) {
		Level_freeBitmap( level->bmps.col );
		Level_freeBitmap( level->bmps.back );
		Level_freeBitmap( level->bmps.fore );
		Level_freeFrames( &level->bmps.stickmen_walking );
		Level_freeFrames( &level->bmps.stickmen_falling );
		Level_freeFrames( &level->bmps.stickmen_dying );
		Level_freeFrames( &level->bmps.stickmen_falldying );
		Level_freeFrames( &level->bmps.stickmen_starting );
		Level_freeFrames( &level->bmps.stickmen_exiting );
		Level_freeFrames( &level->bmps.stickmen_digging );
		Level_freeFrames( &level->bmps.stickmen_building );
		Level_freeFrames( &level->bmps.stickmen_blowing );
		Level_freeFrames( &level->bmps.start );
		Level_freeFrames( &level->bmps.exit );
		Level_freeBitmap( level->bmps.wall );
		Level_freeBitmap( level->bmps.deathzone );
		Level_freeBitmaps( level->bmps.arrow, 9 );
		Level_freeBitmap( level->bmps.capacity_build );
		Level_freeBitmap( level->bmps.capacity_dig );
		Level_freeBitmap( level->bmps.capacity_blow );
		Level_freeBitmap( level->bmps.branch );
		Level_freeBitmaps( level->bmps.stick_fire, 3 );
		Level_freeBitmap( level->bmps.blow );
		Level_freeBitmap( level->bmps.ui_accelerate );
		Level_freeBitmap( level->bmps.ui_pause );
		Level_freeBitmap( level->bmps.ui_menu );

		if( level->bmps.droidsans_14_mono )
			destroy_font( level->bmps.droidsans_14_mono );

		free( level );
	}
}