#ifndef _ECE_SPRITS_CONFIG_H_
#define _ECE_SPRITS_CONFIG_H_

#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

typedef struct GeneralConfig GeneralConfig;
struct GeneralConfig {
	int card;
	int width;
	int height;
	int color_depth;
};

char Config_extractKeyValue( char* line, char* key, char* value );
char Config_extractArray( char* line, char*** values, int* count );
char Config_parseVector2( Vector2* vec, char* value );
char Config_parseCapacitiesNumber( CapacitiesNumber* cap, char* value );

#endif
