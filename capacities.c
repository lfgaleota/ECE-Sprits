#include <stdio.h>
#include "inc/capacities.h"
#include "inc/circular_menu.h"

int Capacities_diggingCallback( Level* level, Object* obj, int x, int y ) {
	if( getpixel( level->bmps.col, x, y ) == COLOR_BREAKABLEWALL ) {
		putpixel( level->bmps.col, x, y, makecol( 0, 0, 0 ) );
		putpixel( level->bmps.fore, x, y, makecol( 255, 0, 255 ) );
	}

	return 0;
}

void Capacities_digging( Level* level, Object* obj ) {
	if( ( obj->oldcp.x - obj->cp.x <= -1 ) || ( obj->oldcp.x - obj->cp.x >= 1 ) || ( obj->oldcp.y - obj->cp.y <= -1 ) || ( obj->oldcp.y - obj->cp.y >= 1 ) ) {
		obj->capacities.left--;
	}

	if( obj->capacities.left > 0 ) {
		obj->state = STATE_DIGGING;

		TrackLine( level, obj, obj->cp.x + obj->capacities.start_points[ 0 ].x, obj->cp.y + obj->capacities.start_points[ 0 ].y, obj->cp.x + obj->capacities.start_points[ 1 ].x, obj->cp.y + obj->capacities.start_points[ 1 ].y, Capacities_diggingCallback );
	} else {
		obj->state = STATE_WALKING;
		obj->capacities.digging = 0;
	}
}

char Capacities_setDirectionCallback( Level* level, Object* obj, int index, int x, int y ) {
	switch( index ) {
		case UP:
			obj->capacities.angle = M_PI_2;
			obj->capacities.distance = obj->size.y / 2;
			return 2;

		case UP_LEFT:
			obj->capacities.angle = 3 * M_PI_4;
			obj->capacities.distance = obj->diagonal;
			return 2;

		case LEFT:
			obj->capacities.angle = M_PI;
			obj->capacities.distance = obj->size.x / 2;
			return 2;

		case DOWN_LEFT:
			obj->capacities.angle = - 3 * M_PI_4;
			obj->capacities.distance = obj->diagonal;
			return 2;

		case DOWN_RIGHT:
			obj->capacities.angle = - M_PI_4;
			obj->capacities.distance = obj->diagonal;
			return 2;

		case RIGHT:
			obj->capacities.angle = 0;
			obj->capacities.distance = obj->size.x / 2;
			return 2;

		case UP_RIGHT:
			obj->capacities.angle = M_PI_4;
			obj->capacities.distance = obj->diagonal;
			return 2;

		case DOWN:
			obj->capacities.angle = - M_PI_2;
			obj->capacities.distance = obj->size.y / 2;
			return 2;

		case 8:
			return 1;
	}

	return 0;
}

char Capacities_setDirection( Level* level, Object* obj ) {
	int prev_mouse_l, mouse_l = 1;
	unsigned char done = 0;
	char ret;

	CircularMenu_updatePosition( level->directions_menu, obj->cp.x, obj->cp.y );

	level->directions_menu->opened = 1;

	BITMAP* save = create_bitmap( SCREEN_W, SCREEN_H );
	if( save )
		blit( screen, save, 0, 0, 0, 0, save->w, save->h );

	while( !done ) {
		if( save )
			blit( save, level->bmps.page, 0, 0, 0, 0, save->w, save->h );

		CircularMenu_show( level->directions_menu, level->bmps.page, mouse_x, mouse_y );

		prev_mouse_l = mouse_l;
		mouse_l = mouse_b & 1;

		if( !prev_mouse_l && mouse_l ) {
			ret = CircularMenu_handleClick( level->directions_menu, level, obj, mouse_x, mouse_y );

			if( ret == 2 ) {
				level->directions_menu->opened = 0;
				done = 2;
			} else if( ret == 1 )  {
				done = 1;
			}
		}

		blit( level->bmps.page, screen, 0, 0, 0, 0, level->bmps.page->w, level->bmps.page->h );
	}

	return done - 1;
}

void Capacities_setDigging( Level* level, Object* obj ) {
	Position start_point;

	if( !Capacities_setDirection( level, obj ) ) {
		return;
	}

	start_point.x = cosf( obj->capacities.angle ) * obj->capacities.distance;
	start_point.y = - sinf( obj->capacities.angle ) * obj->capacities.distance;

	/*obj->capacities.direction.x = cosf( level->capacities.angle ) * 2;
	obj->capacities.direction.y = - sinf( level->capacities.angle ) * 2;*/

	obj->capacities.start_points[ 0 ].x = start_point.x - cosf( obj->capacities.angle + M_PI_2 ) * MAX( obj->size.x, obj->size.y );
	obj->capacities.start_points[ 0 ].y = start_point.y + sinf( obj->capacities.angle + M_PI_2 ) * MAX( obj->size.x, obj->size.y );
	obj->capacities.start_points[ 1 ].x = start_point.x + cosf( obj->capacities.angle + M_PI_2 ) * MAX( obj->size.x, obj->size.y );
	obj->capacities.start_points[ 1 ].y = start_point.y - sinf( obj->capacities.angle + M_PI_2 ) * MAX( obj->size.x, obj->size.y );
	obj->capacities.left = MAX_DIGGING;
	obj->state = STATE_DIGGING;
	obj->capacities.digging = 1;
}

char Capacities_set( Level* level, Object* obj, int index, int x, int y ) {
	switch( index ) {
		case CAPACITY_DIG:
			if( !obj->capacities.digging && !obj->capacities.building && !obj->capacities.blowing ) {
				Capacities_setDigging( level, obj );
			}
			return 1;

		case 3:
			return 1;

		default:
			return 0;
	}
}

void Capacities_update( Level* level, Object* obj ) {
	if( obj->capacities.digging ) {
		Capacities_digging( level, obj );
	}

	if( obj->capacities.building ) {

	}

	if( obj->capacities.blowing ) {

	}

	if( obj->selected ) {
		CircularMenu_updatePosition( level->capacities_menu, obj->cp.x, obj->cp.y );
	}
}