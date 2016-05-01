#include <allegro.h>
#include "inc/init.h"
#include "inc/menu.h"

int main() {
	GeneralConfig config;
	FMod fmod;

	Init( &config );

	FMod_init( &fmod );

	if( !FMod_load( &fmod ) ) {
		allegro_message( "Impossible de charger les sons et musiques." );
		return 1;
	}

	Menu_launch( &fmod, &config );

	FMod_free( &fmod );

	allegro_exit();

	return 0;
}

END_OF_MAIN();