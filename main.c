#include <allegro.h>
#include <loadpng.h>
#include <jpgalleg.h>
#include "inc/init.h"
#include "inc/level.h"
#include "inc/game.h"

int main() {
	Init();

	Level* level = Level_loadStandard( 1 );

	if( level ) {
		Game_launch( level );
	} else {
		allegro_message( "Impossible de charger le niveau 1!" );
	}

	return 0;
}

END_OF_MAIN();