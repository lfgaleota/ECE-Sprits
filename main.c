#include <allegro.h>
#include "inc/init.h"
#include "inc/menu.h"

int main() {
	GeneralConfig config;

	Init( &config );

	Menu_launch( &config );

	allegro_exit();

	return 0;
}

END_OF_MAIN();