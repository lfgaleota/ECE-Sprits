#include "inc/popup_menu.h"

BITMAP* PopupMenu_generateBackground( int width, int height ) {
	int i, j;

	BITMAP* back = create_bitmap( width, height );
	if( !back ) {
		allegro_message( "Erreur d'allocation" );
		return NULL;
	}

	clear_to_color( back, makecol( 255, 0, 255 ) );

	BITMAP* back_texture = load_png( "images/menu/popup/back.png", NULL );
	if( !back_texture ) {
		allegro_message( "Chargement de images/menu/popup/back.png échoué" );
		return NULL;
	}

	BITMAP* top_texture = load_png( "images/menu/popup/top.png", NULL );
	if( !top_texture ) {
		allegro_message( "Chargement de images/menu/popup/top.png échoué" );
		return NULL;
	}

	BITMAP* left_texture = load_png( "images/menu/popup/left.png", NULL );
	if( !left_texture ) {
		allegro_message( "Chargement de images/menu/popup/left.png échoué" );
		return NULL;
	}

	BITMAP* top_left_texture = load_png( "images/menu/popup/top_left.png", NULL );
	if( !top_left_texture ) {
		allegro_message( "Chargement de images/menu/popup/top_left.png échoué" );
		return NULL;
	}

	BITMAP* top_right_texture = load_png( "images/menu/popup/top_right.png", NULL );
	if( !top_right_texture ) {
		allegro_message( "Chargement de images/menu/popup/top_right.png échoué" );
		return NULL;
	}

	BITMAP* bottom_left_texture = load_png( "images/menu/popup/bottom_left.png", NULL );
	if( !bottom_left_texture ) {
		allegro_message( "Chargement de images/menu/popup/bottom_left.png échoué" );
		return NULL;
	}

	BITMAP* bottom_right_texture = load_png( "images/menu/popup/bottom_right.png", NULL );
	if( !bottom_right_texture ) {
		allegro_message( "Chargement de images/menu/popup/bottom_right.png échoué" );
		return NULL;
	}

	for( i = MENU_BACK_OFFSET; i < height - MENU_BACK_OFFSET; i += back_texture->h ) {
		for( j = MENU_BACK_OFFSET; j < width - MENU_BACK_OFFSET; j += back_texture->w ) {
			masked_blit( back_texture, back, 0, 0, j, i, back_texture->w, back_texture->h );
		}
	}

	for( j = MENU_BACK_OFFSET; j < width - MENU_BACK_OFFSET; j += top_texture->w ) {
		masked_blit( top_texture, back, 0, 0, j, MENU_LEFT_BAR_OFFSET, top_texture->w, top_texture->h );
		masked_blit( top_texture, back, 0, 0, j, height - MENU_LEFT_BAR_OFFSET - top_texture->h, top_texture->w, top_texture->h );
	}

	for( i = MENU_BACK_OFFSET; i < height - MENU_BACK_OFFSET; i += left_texture->h ) {
		masked_blit( left_texture, back, 0, 0, MENU_TOP_BAR_OFFSET, i, left_texture->w, left_texture->h );
		masked_blit( left_texture, back, 0, 0, width - MENU_TOP_BAR_OFFSET - left_texture->w, i, left_texture->w, left_texture->h );
	}

	rectfill( back, 0, 0, MENU_TOP_BAR_OFFSET, back->h, makecol( 255, 0, 255 ) );
	rectfill( back, 0, 0, back->w, MENU_LEFT_BAR_OFFSET, makecol( 255, 0, 255 ) );
	rectfill( back, 0, back->h - MENU_TOP_BAR_OFFSET, back->w, back->h, makecol( 255, 0, 255 ) );
	rectfill( back, back->w - MENU_LEFT_BAR_OFFSET, 0, back->w, back->h, makecol( 255, 0, 255 ) );

	masked_blit( top_left_texture, back, 0, 0, 0, 0, top_left_texture->w, top_left_texture->h );
	masked_blit( top_right_texture, back, 0, 0, width - top_right_texture->w, 0, top_right_texture->w, top_right_texture->h );
	masked_blit( bottom_left_texture, back, 0, 0, 0, height - bottom_left_texture->h, bottom_left_texture->w, bottom_left_texture->h );
	masked_blit( bottom_right_texture, back, 0, 0, width - bottom_right_texture->w, height - bottom_right_texture->h, bottom_right_texture->w, bottom_right_texture->h );

	destroy_bitmap( back_texture );
	destroy_bitmap( top_texture );
	destroy_bitmap( left_texture );
	destroy_bitmap( top_left_texture );
	destroy_bitmap( top_right_texture );
	destroy_bitmap( bottom_left_texture );
	destroy_bitmap( bottom_right_texture );

	return back;
}

int PopupMenu_open( char* title, char** choices, int count, FONT* fonttext ) {
	BITMAP *back, *page, *save;
	int width, height, i, choice = 0, quit = 0, start_x, start_y, end_x, selected_button;
	volatile char prev_mouse_l, mouse_l = 1, prev_key_down, key_down = 1, prev_key_up, key_up = 1, prev_key_enter, key_enter = 1, prev_key_esc, key_esc = 1;
	size_t size, max_size;
	char font_loaded = 0;

	if( fonttext == NULL ) {
		fonttext = load_font( "polices/droidsans_14_mono.pcx", NULL, NULL );
		font_loaded = 1;

		if( fonttext == NULL ) {
			font_loaded = 0;
			allegro_message( "Imoossible de charger la police pour le menu." );
			fonttext = font;
		}
	}

	height = MENU_PADDING_Y * 2 + ( count + 1 ) * MENU_LETTER_SPACING_Y + ( count + 2 ) * MENU_FONT_SIZE_Y;
	width = MENU_PADDING_X * 2;
	max_size = strlen( title );

	for( i = 0; i < count; i++ ) {
		size = strlen( choices[ i ] );
		if( size > max_size )
			max_size = size;
	}

	width += max_size * MENU_FONT_SIZE_X * MENU_LETTER_SPACING_X;

	back = PopupMenu_generateBackground( width, height );
	if( !back ) {
		return 0;
	}

	page = create_bitmap( SCREEN_W, SCREEN_H );
	if( !page ) {
		allegro_message( "Erreur d'allocation" );
		return 0;
	}

	save = create_bitmap( SCREEN_W, SCREEN_H );
	if( save )
		blit( screen, save, 0, 0, 0, 0, SCREEN_W, SCREEN_H );

	start_y = ( SCREEN_H - height ) / 2 +  2 * MENU_PADDING_Y + MENU_FONT_SIZE_Y;
	start_x = ( SCREEN_W - width ) / 2 + MENU_PADDING_X;
	end_x = ( SCREEN_W + width ) / 2 - MENU_PADDING_X;

	while( !quit ) {
		prev_mouse_l = mouse_l;
		mouse_l = (unsigned char) mouse_b & 1;

		prev_key_down = key_down;
		key_down = key[ KEY_DOWN ];

		prev_key_up= key_up;
		key_up = key[ KEY_UP ];

		prev_key_enter = key_enter;
		key_enter = key[ KEY_ENTER ];

		prev_key_esc = key_esc;
		key_esc = key[ KEY_ESC ];

		if( !prev_key_enter && key_enter )
			quit = 1;

		if( !prev_key_up && key_up ) {
			if( choice > 1 )
				choice--;
		}

		if( !prev_key_down && key_down ) {
			if( choice < count )
				choice++;
		}

		if( !prev_key_esc && key_esc ) {
			choice = 0;
			quit = 1;
		}

		if( save )
			blit( save, page, 0, 0, 0, 0, SCREEN_W, SCREEN_H );

		masked_blit( back, page, 0, 0, ( SCREEN_W - width ) / 2, ( SCREEN_H - height ) / 2, SCREEN_W, SCREEN_H );

		if( mouse_x >= start_x && mouse_x <= end_x ) {
			selected_button = ( mouse_y - start_y ) / ( MENU_FONT_SIZE_Y + MENU_LETTER_SPACING_Y );
			if( selected_button >= 0 && selected_button < count ) {
				choice = selected_button + 1;
			}

			if( !prev_mouse_l && mouse_l ) {
				quit = 1;
			}
		}

		textprintf_centre_ex( page, fonttext, SCREEN_W / 2, ( SCREEN_H - height ) / 2 + MENU_PADDING_Y, makecol( 255, 255, 255 ), -1, title );

		for( i = 0; i < count; i++ ) {
			if( i == choice - 1 )
				textprintf_centre_ex( page, fonttext, SCREEN_W / 2, ( SCREEN_H - height ) / 2 + MENU_PADDING_Y + ( i + 2 ) * ( MENU_FONT_SIZE_Y + MENU_LETTER_SPACING_Y ), makecol( 255, 255, 255 ), -1, choices[ i ] );
			else
				textprintf_centre_ex( page, fonttext, SCREEN_W / 2, ( SCREEN_H - height ) / 2 + MENU_PADDING_Y + ( i + 2 ) * ( MENU_FONT_SIZE_Y + MENU_LETTER_SPACING_Y ), makecol( 255, 255, 0 ), -1, choices[ i ] );
		}

		blit( page, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H );
	}

	destroy_bitmap( back );
	if( save )
		destroy_bitmap( save );
	destroy_bitmap( page );

	if( font_loaded )
		destroy_font( fonttext );

	return choice;
}

void PopupMenu_alert( char* title, char* button, FONT* fonttext ) {
	char font_loaded = 0;

	if( fonttext == NULL ) {
		fonttext = load_font( "polices/droidsans_14_mono.pcx", NULL, NULL );
		font_loaded = 1;

		if( fonttext == NULL ) {
			font_loaded = 0;
			allegro_message( "Imoossible de charger la police pour le menu." );
			fonttext = font;
		}
	}

	if( button == NULL ) {
		char* choices[] = { "OK" };

		PopupMenu_open( title, choices, 1, fonttext );
	} else {
		char* choices[] = { button };

		PopupMenu_open( title, choices, 1, fonttext );
	}

	if( font_loaded ) {
		destroy_font( fonttext );
	}
}
