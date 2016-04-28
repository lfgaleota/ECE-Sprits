#include <allegro.h>
#include <loadpng.h>
#include <jpgalleg.h>
#include "inc/init.h"
#include "inc/level.h"
#include "inc/game.h"

int main() {
	Init();

	Level level;

	level.bmps.col = load_png( "niveau1/collision.png", NULL );
	if( !level.bmps.col ) {
		allegro_message( "Erreur chargement niveau1/collision.png" );
		return EXIT_FAILURE;
	}

	level.bmps.back = load_jpg( "niveau1/background.jpg", NULL );
	if( !level.bmps.back ) {
		allegro_message( "Erreur chargement niveau1/background.jpg" );
		return EXIT_FAILURE;
	}

	level.bmps.fore = load_png( "niveau1/foreground.png", NULL );
	if( !level.bmps.fore ) {
		allegro_message( "Erreur chargement niveau1/foreground.png" );
		return EXIT_FAILURE;
	}

	level.nb_stickmen_max = 10;
	level.nb_stickmen_should_arrive = 9;
	level.counter_stickmen_arrival_max = 50;

	level.bmps.stickmen_walking = Level_loadFrames( "images/sticks/stickMan", 20 );
	level.bmps.stickmen_falling = Level_loadFrames( "images/sticks/stickManChute", 1 );
	level.bmps.stickmen_dying = Level_loadFrames( "images/sticks/stickManMort", 5 );
	level.bmps.stickmen_falldying = Level_loadFrames( "images/sticks/stickManMortChute", 5 );
	level.bmps.start = Level_loadFrames( "images/portails/portailDebut", 1 );
	level.bmps.exit = Level_loadFrames( "images/portails/portailArrivee", 1 );

	level.bmps.wall = load_jpg( "niveau1/texture_wall.jpg", NULL );
	level.bmps.deathzone = load_jpg( "niveau1/texture_deathzone.jpg", NULL );

	level.bmps.arrow = Level_loadBitmaps( "images/arrows/arrow", 9 );

	level.bmps.capacity_build = load_png( "images/capacites/construire.png", NULL );
	level.bmps.capacity_dig = load_png( "images/capacites/creuser.png", NULL );
	level.bmps.capacity_blow = load_png( "images/capacites/souffler.png", NULL );

	if( !level.bmps.stickmen_walking.bmps || !level.bmps.stickmen_falling.bmps || !level.bmps.stickmen_dying.bmps || !level.bmps.stickmen_falldying.bmps || !level.bmps.start.bmps || !level.bmps.exit.bmps ) {
		allegro_message( "CATASTROPHE" );
		return EXIT_FAILURE;
	}

	level.fast_dt = 2;
	level.slow_dt = 1;

	level.start = (Vector2) { 110, 60 };
	level.exit = (Vector2) { 99, 649 };

	level.gravity = (Vector2) { 0, 4 };
	level.movement = (Vector2) { 1, 0 };

	level.capacities = (CapacitiesNumber) { 1, 1, 1 };

	Game_launch( &level );

	return 0;
}

END_OF_MAIN();