#include <allegro.h>
#include "inc/init.h"
#include "inc/menu.h"

int main() {
	Init();

	Menu_launch();

	allegro_exit();

	return 0;
}

END_OF_MAIN();