#include "inc/circular_menu.h"

CircularMenu *CircularMenu_create( int x, int y, int radius, int size, MenuItem *items, int count, FONT* font ) {
	CircularMenu *menu = malloc( sizeof( CircularMenu ) );
	if( !menu ) {
		allegro_message( "Allocation du menu échouée." );
		return NULL;
	}

	if( radius <= 0 || size <= 0 || !items || count <= 0 || !font ) {
		allegro_message( "Configuration du menu circulaire invalide." );
		return NULL;
	}

	menu->font = font;
	menu->count = count;
	menu->inner_radius = radius;
	menu->medium_radius = radius + size / 2;
	menu->outer_radius = radius + size;
	menu->screen_x = x - menu->outer_radius;
	menu->screen_y = y - menu->outer_radius;
	menu->items = items;
	menu->opened = 0;

	return menu;
}

void CircularMenu_compute( CircularMenu *menu ) {
	int i;
	float mid_angle;
	MenuItem *item;

	if( menu ) {
		menu->bmp = create_bitmap( 2 * menu->outer_radius, 2 * menu->outer_radius );
		if( !menu->bmp ) {
			allegro_message( "Allocation de la bitmap du menu échouée." );
			return;
		}

		menu->angle = 2 * M_PI / menu->count;

		rectfill( menu->bmp, 0, 0, menu->bmp->w, menu->bmp->h, makecol( 255, 0, 255 ) );
		circlefill( menu->bmp, menu->outer_radius, menu->outer_radius, menu->outer_radius, 0 );
		circlefill( menu->bmp, menu->outer_radius, menu->outer_radius, menu->inner_radius,
		            makecol( 255, 0, 255 ) );

		for( i = 0; i < menu->count; i++ ) {
			item = &menu->items[ i ];

			mid_angle = ( menu->angle * ( i + 1 ) + menu->angle * i ) / 2;
			item->center_x = menu->outer_radius + menu->medium_radius * cosf( mid_angle );
			item->center_y = menu->outer_radius + menu->medium_radius * sinf( mid_angle );

			line( menu->bmp, menu->outer_radius, menu->outer_radius,
			      menu->outer_radius + menu->outer_radius * cosf( menu->angle * i ),
			      menu->outer_radius + menu->outer_radius * sinf( menu->angle * i ), makecol( 255, 0, 255 ) );
			line( menu->bmp, menu->outer_radius, menu->outer_radius,
			      menu->outer_radius + menu->outer_radius * cosf( menu->angle * ( i + 1 ) ),
			      menu->outer_radius + menu->outer_radius * sinf( menu->angle * ( i + 1 ) ), makecol( 255, 0, 255 ) );
			floodfill( menu->bmp, item->center_x, item->center_y, item->bg_color );

			item->icon_x = menu->screen_x + item->center_x - item->icon->w / 2;
			item->icon_y = menu->screen_y + item->center_y - item->icon->w / 2;
		}
	}
}

void CircularMenu_recompute( CircularMenu *menu ) {
	if( menu ) {
		if( menu->bmp ) {
			destroy_bitmap( menu->bmp );
			menu->bmp = NULL;
		}

		CircularMenu_compute( menu );
	}
}

void CircularMenu_updatePosition( CircularMenu* menu, int x, int y ) {
	int i;

	if( menu ) {
		menu->screen_x = x - menu->outer_radius;
		menu->screen_y = y - menu->outer_radius;

		for( i = 0; i < menu->count; i++ ) {
			menu->items[ i ].icon_x = menu->screen_x + menu->items[ i ].center_x - menu->items[ i ].icon->w / 2;
			menu->items[ i ].icon_y = menu->screen_y + menu->items[ i ].center_y - menu->items[ i ].icon->w / 2;
		}
	}
}

void CircularMenu_show( CircularMenu* menu, BITMAP* dest, int x, int y ) {
	int i, col = 0, tooltip_i = -1;

	if( menu && menu->opened ) {
		if( ( x >= menu->screen_x ) && ( y >= menu->screen_y ) && ( x <= ( menu->screen_x + menu->bmp->w ) ) &&
		    ( y <= ( menu->screen_y + menu->bmp->h ) ) ) {
			col = getpixel( menu->bmp, x - menu->screen_x, y - menu->screen_y );
		}

		set_trans_blender( 255, 0, 255, 0 );
		masked_blit( menu->bmp, dest, 0, 0, menu->screen_x, menu->screen_y, menu->bmp->w, menu->bmp->h );

		for( i = 0; i < menu->count; i++ ) {
			if( col > 0 && col == menu->items[ i ].bg_color ) {
				floodfill( dest, x, y, menu->items[ i ].bg_sel_color );
				tooltip_i = i;
			}

			set_alpha_blender();
			draw_trans_sprite( dest, menu->items[ i ].icon, menu->items[ i ].icon_x, menu->items[ i ].icon_y );
		}

		if( tooltip_i > -1 )
			textprintf_ex( dest, menu->font, x + menu->font->height, y, menu->items[ tooltip_i ].text_color, menu->items[ tooltip_i ].bg_sel_color, "%s", menu->items[ tooltip_i ].tooltip );
	}
}

char CircularMenu_handleClick( CircularMenu* menu, Level* level, Object* obj, int x, int y ) {
	int i, col;
	char ret = 0;

	if( menu ) {
		col = getpixel( menu->bmp, x - menu->screen_x, y - menu->screen_y );

		if( menu->opened ) {
			for( i = 0; i < menu->count; i++ ) {
				if( col > 0 && col == menu->items[ i ].bg_color ) {
					ret = menu->items[ i ].callback( level, obj, i, mouse_x, mouse_y );
				}
			}
		}
	}

	return ret;
}

void CircularMenu_free( CircularMenu* menu, char free_items, char free_items_bitmaps, char free_items_tooltips ) {
	int i;

	if( menu ) {
		destroy_bitmap( menu->bmp );

		if( free_items_bitmaps ) {
			for( i = 0; i < menu->count; i++ ) {
				if( menu->items[ i ].icon )
					destroy_bitmap( menu->items[ i ].icon );
			}
		}

		if( free_items_tooltips ) {
			for( i = 0; i < menu->count; i++ ) {
				free( menu->items[ i ].tooltip );
			}
		}

		if( free_items ) {
			free( menu->items );
		}
	}
}