#include "inc/init.h"

void Init() {
	allegro_init();

	register_png_file_type();
	install_keyboard();
	install_mouse();

	set_color_depth( desktop_color_depth() );
	if( set_gfx_mode( GFX_AUTODETECT_WINDOWED, 1024, 768, 0, 0 ) != 0 ) {
		allegro_message( "probleme mode graphique" );
		allegro_exit();
		exit( EXIT_FAILURE );
	}

	set_window_title( "ECE-Sprits" );

	srand( time( NULL ) );
}