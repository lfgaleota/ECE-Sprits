#include "save.h"

char Save_save( Save* save ) {
	FILE* file;

	char path[] = "saves/XXX";
	sprintf( path, "saves/%d", save->id );

	file = fopen( path, "w" );
	if( !file )
		return 0;

	fprintf( file, "%c", save->standard_level );

	fclose( file );

	return 1;
}

Save Save_load( unsigned char id ) {
	Save save;
	FILE* file;

	char path[] = "saves/XXX";
	sprintf( path, "saves/%d", id );

	save.id = id;

	file = fopen( path, "r" );
	if( !file ) {
		save.standard_level = 0;
		Save_save( &save );
		return save;
	}

	fscanf( file, "%c", &save.standard_level );
	fclose( file );

	return save;
}