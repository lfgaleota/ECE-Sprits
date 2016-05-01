#ifndef _ECE_SPRITS_LEVEL_H_
#define _ECE_SPRITS_LEVEL_H_

#include <stdio.h>
#include <allegro.h>
#include <loadpng.h>
#include <jpgalleg.h>
#include "structures.h"
#include "config.h"

BITMAP** Level_loadBitmaps( char* path, unsigned short count );
Frames Level_loadFrames( char* path, unsigned short count );

char Level_loadProperties( char* path, Level* level );

void Level_parseConfigurationLine( Level* level, char* line, int* lines_parsed );
char Level_extractKeyValue( char* line, char* key, char* value );
char Level_extractArray( char* line, char*** values, int* count );
char Level_parseCapacitiesNumber( CapacitiesNumber* cap, char* value );
char Level_parseVector2( Vector2* vec, char* value );

Level* Level_load( char* path );
Level* Level_loadStandard( int number );

void Level_freeBitmap( BITMAP* bmp );
void Level_freeBitmaps( BITMAP** bmps, unsigned short count );
void Level_freeFrames( Frames* frames );
void Level_free( Level* level );

#endif //_ECE_SPRITS_LEVEL_H_
