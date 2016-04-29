#include "inc/popup_menu.h"

BITMAP* Menu_generateBackground( int width, int height ) {
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

void Menu_open( FONT* fonttext, char** choices, int count ) {
	BITMAP *back, *page, *save;
	int width, height, i, choice = 0, quit = 0, start_x, start_y, end_x, selected_button;
	size_t size, max_size;

	height = MENU_PADDING_Y * 2 + ( count - 1 ) * MENU_LETTER_SPACING_Y + count * MENU_FONT_SIZE;
	width = MENU_PADDING_X * 2;
	max_size = 0;

	usleep( 100000 );

	for( i = 0; i < count; i++ ) {
		size = strlen( choices[ i ] );
		if( size > max_size )
			max_size = size;
	}

	width += max_size * MENU_FONT_SIZE * MENU_LETTER_SPACING_X;

	back = Menu_generateBackground( width, height );
	if( !back ) {
		return;
	}

	page = create_bitmap( SCREEN_W, SCREEN_H );
	if( !page ) {
		allegro_message( "Erreur d'allocation" );
		return;
	}

	save = create_bitmap( SCREEN_W, SCREEN_H );
	if( save )
		blit( screen, save, 0, 0, 0, 0, SCREEN_W, SCREEN_H );

	start_y = ( SCREEN_H - height ) / 2 + MENU_PADDING_Y;
	start_x = ( SCREEN_W - width ) / 2 + MENU_PADDING_X;
	end_x = ( SCREEN_W + width ) / 2 - MENU_PADDING_X;

	while( !quit ) {
		if( key[ KEY_ENTER ] )
			quit = 1;

		if( key[ KEY_ESC ] ) {
			quit = 1;
			choice = 0;
		}

		if( save )
			blit( save, page, 0, 0, 0, 0, SCREEN_W, SCREEN_H );

		masked_blit( back, page, 0, 0, ( SCREEN_W - width ) / 2, ( SCREEN_H - height ) / 2, SCREEN_W, SCREEN_H );

		if( mouse_x >= start_x && mouse_x <= end_x ) {
			selected_button = ( mouse_y - start_y ) / ( MENU_FONT_SIZE + MENU_LETTER_SPACING_Y );
			if( selected_button >= 0 && selected_button < count ) {
				choice = selected_button + 1;
			}
		}

		for( i = 0; i < count; i++ ) {
			if( i == choice - 1 )
				textprintf_centre_ex( page, fonttext, SCREEN_W / 2, ( SCREEN_H - height ) / 2 + MENU_PADDING_Y + i * ( MENU_FONT_SIZE + MENU_LETTER_SPACING_Y ), makecol( 255, 255, 255 ), -1, choices[ i ] );
			else
				textprintf_centre_ex( page, fonttext, SCREEN_W / 2, ( SCREEN_H - height ) / 2 + MENU_PADDING_Y + i * ( MENU_FONT_SIZE + MENU_LETTER_SPACING_Y ), makecol( 255, 255, 0 ), -1, choices[ i ] );
		}

		blit( page, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H );
	}

	destroy_bitmap( back );
	if( save )
		destroy_bitmap( save );
	destroy_bitmap( page );
}