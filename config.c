#include "inc/config.h"

char Config_extractKeyValue( char* line, char* key, char* value ) {
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

char Config_extractArray( char* line, char*** values, int* count ) {
	char** newvalues;
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

	newvalues = calloc( (size_t) (*count), sizeof( char* ) );
	if( !newvalues )
		return 0;

	for( i = 0, token = strtok( buffer, "," ); i < *count && token != NULL; i++ ) {
		newvalues[ i ] = calloc( strlen( token ) + 1, sizeof( char ) );
		if( !newvalues[ i ] )
			return 0;

		strcpy( newvalues[ i ], token );
		token = strtok( NULL, "," );
	}

	*values = newvalues;

	return 1;
}

char Config_parseVector2( Vector2* vec, char* value ) {
	int i, count = 0;
	char ret = 0, **values = NULL;

	if( !Config_extractArray( value, &values, &count ) )
		return 0;

	if( values == NULL )
		return 0;

	if( count > 1 ) {
		*vec = (Vector2) { strtol( values[ 0 ], NULL, 10 ), strtol( values[ 1 ], NULL, 10 ) };
		ret = 1;
	}

	for( i = 0; i < count; i++ ) {
		if( values[ i ] )
			free( values[ i ] );
	}
	free( values );

	return ret;
}

char Config_parseCapacitiesNumber( CapacitiesNumber* cap, char* value ) {
	int i, count = 0;
	char ret = 0, **values = NULL;

	if( !Config_extractArray( value, &values, &count ) )
		return 0;

	if( values == NULL )
		return 0;

	if( count > 2 ) {
		*cap = (CapacitiesNumber) { strtoul( values[ 0 ], 0, 10 ), strtoul( values[ 1 ], 0, 10 ), strtoul( values[ 2 ], 0, 10 ) };
		ret = 1;
	}

	for( i = 0; i < count; i++ ) {
		if( values[ i ] )
			free( values[ i ] );
	}

	free( values );

	return ret;
}

void GeneralConfig_parseConfigurationLine( GeneralConfig* config, char* line, int* lines_parsed ) {
	char key[ BUFFER_SIZE ], value[ BUFFER_SIZE ];

	if( !config || !line )
		return;

	if( Config_extractKeyValue( line, key, value ) ) {
		if( strcmp( key, "card" ) == 0 ) {
			config->card = (int) strtoul( value, 0, 10 );
			(*lines_parsed)++;
		} else if( strcmp( key, "width" ) == 0 ) {
			config->width  = (int) strtoul( value, 0, 10 );
			(*lines_parsed)++;
		} else if( strcmp( key, "height" ) == 0 ) {
			config->height  = (int) strtoul( value, 0, 10 );
			(*lines_parsed)++;
		} else if( strcmp( key, "color_depth" ) == 0 ) {
			config->color_depth  = (int) strtoul( value, 0, 10 );
			(*lines_parsed)++;
		}
	}
}

char GeneralConfig_load( GeneralConfig* config ) {
	FILE* properties;
	int lines_parsed = 0;
	char buffer[ BUFFER_SIZE ];

	properties = fopen( "config", "r" );

	if( !properties ) {
		return 0;
	}

	while( fgets( buffer, BUFFER_SIZE, properties ) != NULL )  {
		GeneralConfig_parseConfigurationLine( config, buffer, &lines_parsed );
	}

	if( lines_parsed == 4 )
		return 1;

	return 0;
}

char GeneralConfig_save( GeneralConfig* config ) {
	FILE* properties;

	properties = fopen( "config", "w" );

	if( !properties ) {
		return 0;
	}

	fprintf( properties, "card:%d\nwidth:%d\nheight:%d\ncolor_depth:%d", config->card, config->width, config->height, config->color_depth );

	return 0;
}