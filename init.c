#include "inc/init.h"

void Init( GeneralConfig* config ) {
	allegro_init();

	jpgalleg_init();
	register_png_file_type();
	install_keyboard();
	install_mouse();

	if( !GeneralConfig_load( config ) ) {
		config->card = GFX_AUTODETECT_WINDOWED;
		config->width = 1024;
		config->height = 768;
		config->color_depth = desktop_color_depth();
		GeneralConfig_save( config );
	}

	set_color_depth( config->color_depth );
	if( set_gfx_mode( config->card, config->width, config->height, 0, 0 ) != 0 ) {
		allegro_message( "probleme mode graphique" );
		allegro_exit();
		exit( EXIT_FAILURE );
	}

	set_window_title( "ECE-Sprits" );

	enable_hardware_cursor();
	select_mouse_cursor( 2 );
	show_mouse( screen );

	srand( time( NULL ) );
}