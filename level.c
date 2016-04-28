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