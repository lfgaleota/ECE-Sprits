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

	if( !level->bmps.stickmen_walking.bmps || !level->bmps.stickmen_falling.bmps || !level->bmps.stickmen_dying.bmps || !level->bmps.stickmen_falldying.bmps || !level->bmps.stickmen_starting.bmps || !level->bmps.stickmen_exiting.bmps || !level->bmps.start.bmps || !level->bmps.exit.bmps || !level->bmps.arrow || !level->bmps.capacity_build || !level->bmps.capacity_dig || !level->bmps.capacity_blow || !level->bmps.branch ) {
		allegro_message( "Impossible de charger les images" );
		return 0;
	}

	level->bmps.droidsans_14_mono = load_font( "polices/droidsans_14_mono.pcx", NULL, NULL );

	return 1;
}

char Level_extractKeyValue( char* line, char* key, char* value ) {
	char c;
	size_t i;

	if( !line || !key || !value )
		return 0;

	for( i = 0; i < BUFFER_SIZE - 1 && line[ i ] != 0; i++ ) {
		c = line[ i ];

		if( c == ':' ) {
			strncpy( key, line, i );
			key[ i ] = 0;
			strncpy( value, line + i + 1, BUFFER_SIZE - i );
			value[ BUFFER_SIZE - i ] = 0;
			return 1;
		}
	}

	return 0;
}

char Level_extractArray( char* line, char*** values, int* count ) {
	char buffer[ BUFFER_SIZE ], buffer2[ BUFFER_SIZE ], c, *token;
	int i, start_index = -1, end_index = -1;

	if( !line || !count )
		return 0;

	*count = 0;

	for( i = 0; i < BUFFER_SIZE - 1 && line[ i ] != 0; i++ ) {
		c = line[ i ];

		if( c == '(' ) {
			start_index = i + 1;
		} else if( c == ')' && start_index != -1 ) {
			end_index = i;
		}
	}

	if( start_index < 0 || end_index < 0 )
		return 0;

	strncpy( buffer, line + start_index, (size_t) end_index - start_index );
	buffer[ end_index - start_index ] = 0;
	strcpy( buffer2, buffer );

	token = strtok( buffer2, "," );
	while( token != NULL ) {
		(*count)++;
		token = strtok( NULL, "," );
	}

	*values = malloc( sizeof( char* ) * (*count) );
	if( !*values )
		return 0;

	for( i = 0, token = strtok( buffer, "," ); i < *count && token != NULL; i++ ) {
		(*values)[ i ] = malloc( sizeof( char ) * strlen( token ) );
		if( !(*values)[ i ] )
			return 0;

		strcpy( (*values)[ i ], token );
		token = strtok( NULL, "," );
	}

	return 1;
}

char Level_parseVector2( Vector2* vec, char* value ) {
	int i, count = 0;
	char ret = 0, **values = NULL;

	if( !Level_extractArray( value, &values, &count ) )
		return 0;

	if( count > 1 ) {
		*vec = (Vector2) { strtol( values[ 0 ], NULL, 10 ), strtol( values[ 1 ], NULL, 10 ) };
		ret = 1;
	}

	for( i = 0; i < count; i++ ) {
		free( values[ i ] );
	}
	free( values );

	return ret;
}

char Level_parseCapacitiesNumber( CapacitiesNumber* cap, char* value ) {
	int i, count = 0;
	char ret = 0, **values = NULL;

	if( !Level_extractArray( value, &values, &count ) )
		return 0;

	if( count > 2 ) {
		*cap = (CapacitiesNumber) { strtoul( values[ 0 ], 0, 10 ), strtoul( values[ 1 ], 0, 10 ), strtoul( values[ 2 ], 0, 10 ) };
		ret = 1;
	}

	if( values ) {
		for( i = 0; i < count; i++ ) {
			free( values[ i ] );
		}

		free( values );
	}

	return ret;
}

void Level_parseConfigurationLine( Level* level, char* line, int* lines_parsed ) {
	char key[ BUFFER_SIZE ], value[ BUFFER_SIZE ];

	if( !level || !line )
		return;

	if( Level_extractKeyValue( line, key, value ) ) {
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
			if( Level_parseVector2( &level->start, value ) )
				(*lines_parsed)++;
		} else if( strcmp( key, "exit" ) == 0 ) {
			if( Level_parseVector2( &level->exit, value ) )
				(*lines_parsed)++;
		} else if( strcmp( key, "gravity" ) == 0 ) {
			if( Level_parseVector2( &level->gravity, value ) )
				(*lines_parsed)++;
		} else if( strcmp( key, "movement" ) == 0 ) {
			if( Level_parseVector2( &level->movement, value ) )
				(*lines_parsed)++;
		} else if( strcmp( key, "capacities" ) == 0 ) {
			if( Level_parseCapacitiesNumber( &level->capacities, value ) )
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

	if( lines_parsed == 10 )
		return 1;

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