#include "inc/collisions.h"

char Collision_wallCallback( Object* obj, char side, unsigned char status ) {
	char ret = 0;

	/*Vector2 normal;

	if( getpixel( col, x - 1, y - 1 ) == makecol( 255, 255, 255 ) && getpixel( col, x + 1, y + 1 ) == makecol( 255, 255, 255 )  ) {
		normal = (Vector2) { 1, -1 };
	} else if( getpixel( col, x - 1, y + 1 ) == makecol( 255, 255, 255 ) && getpixel( col, x + 1, y - 1 ) == makecol( 255, 255, 255 )  ) {
		normal = (Vector2) { -1, -1 };
	} else {
		normal = (Vector2) { 1, 1 };
	}

	if( obj->dir.x == 0 ) {
		normal.x *= obj->dir.y;
		normal.y *= obj->dir.y;
	} else if( obj->dir.y == 0 ) {
		normal.x *= obj->dir.x;
		normal.y *= obj->dir.x;
	} else {
		normal.x *= obj->dir.x;
		normal.y *= obj->dir.y;
	}*/

	if( !( status & STATUS_ALREADY_BLOCKED ) ) {
		if( ( side == SIDE_UP && obj->dir.y <= 0 ) || ( side == SIDE_DOWN && obj->dir.y >= 0 ) ) {
			obj->v.y = 0;
			obj->force.y = 0;
			ret |= STATUS_ALREADY_BLOCKED;
		}

		if( status & STATUS_IS_SIDE_OF_FLYING ) {
			if( obj->v.y > Y_FALLDAMAGE ) {
				obj->state = STATE_FALLDYING;
			}
		}

		if( side == SIDE_LEFT && !( status & STATUS_WALL ) && obj->v.x != 0 ) {
			obj->v.x = 0;
			obj->force.x = 0;
			obj->direction = 1;
			ret |= STATUS_ALREADY_BLOCKED;
			//printf("Blocked because second wall!\n");
		}

		if( side == SIDE_RIGHT && !( status & STATUS_WALL ) && obj->v.x != 0 ) {
			obj->v.x = 0;
			obj->force.x = 0;
			obj->direction = 0;
			ret |= STATUS_ALREADY_BLOCKED;
			//printf("Blocked because second wall!\n");
		}
	}

	//printf( "-> !! Pixel detected at %d, %d !!\n", x, y );

	// Calculate relative velocity in terms of the normal direction
	/*float v_along_normal = obj->v.x * normal.x + obj->v.y * normal.y;

	// Do not resolve if velocities are separating
	if( v_along_normal > 0 ) {
		printf( "-> !! Separating velocity !!\n" );
		return ret;
	}

	float j = -( 1 + obj->restitution ) * v_along_normal / obj->mass / obj->size.x;

	// Apply impulse
	Vector2 impulse = (Vector2) { normal.x * j, normal.y * j };

	Vector2 r = ( Vector2 ) { x - obj->cp.x, y - obj->cp.y };
	obj->torque = r.x * impulse.y - r.y * impulse.x;

	obj->force.x += impulse.x;
	obj->force.y += impulse.y;*/

	return ret;
}

void Collision_exitCallback( Object* obj, unsigned char status ) {
	if( status & STATUS_IS_SIDE_OF_FLYING || status & STATUS_IS_SIDE_OF_WALKING ) {
		obj->state = STATE_EXITING;
		obj->counter = 0;
	}
}

void Collision_deathZoneCallback( Object* obj, unsigned char status ) {
	if( status & STATUS_IS_SIDE_OF_FLYING || status & STATUS_IS_SIDE_OF_WALKING ) {
		obj->state = STATE_DYING;
		obj->counter = 0;
	}
}

char Collision_callback( Object* obj, BITMAP* col, int x, int y, char side, unsigned char status, int progression_y ) {
	int px = getpixel( col, x, y );
	char ret = 0;

	if( px == COLOR_WALL || px == COLOR_BREAKABLEWALL ) {
		ret = Collision_wallCallback( obj, side, status );
	} else {
		ret |= STATUS_WALL;

		if( px == COLOR_EXIT ) {
			Collision_exitCallback( obj, status );
		}

		if( px == COLOR_DEATHZONE ) {
			Collision_deathZoneCallback( obj, status );
		}

		if( status & STATUS_IS_SIDE_OF_WALKING && progression_y > 0 && obj->dir.x != 0 ) {
			if( status & STATUS_WALL ) {
				//printf( "Status: Y: %d, Wall: %d, Blocked: %d\n", progression_y, status & STATUS_WALL, status & STATUS_ALREADY_BLOCKED );
				if( progression_y < MAX_STEP ) {
					obj->propcp.y += progression_y;
					//printf("Should go up!\n");
				} else {
					obj->v.x = 0;
					obj->force.x = 0;
					if( side == SIDE_LEFT )
						obj->direction = 1;
					else
						obj->direction = 0;
					ret |= STATUS_ALREADY_BLOCKED;
					//printf("Blocked because wall too high!\n");
				}
			}
		}
	}

	return ret;
}

void Collision_trackLine( Object* obj, BITMAP* col, int x1, int y1, int x2, int y2, char side ) {
	int dx, dy, dx_dir, dy_dir;
	char ret;
	unsigned char status = 0x2;
	float i, j, m;

	dx = x2 - x1;
	dy = y2 - y1;

	if( dx > 0 )
		dx_dir = 1;
	else if( dx < 0 )
		dx_dir = -1;
	else
		dx_dir = 0;
	if( dy > 0 )
		dy_dir = 1;
	else if( dy < 0 )
		dy_dir = -1;
	else
		dy_dir = 0;

	if( ( side == SIDE_LEFT && obj->dir.x < 0 ) || ( side == SIDE_RIGHT && obj->dir.x > 0 ) )
		status |= STATUS_IS_SIDE_OF_WALKING;

	if( ( side == SIDE_UP && obj->dir.y < 0 ) || ( side == SIDE_DOWN && obj->dir.y > 0 ) )
		status |= STATUS_IS_SIDE_OF_FLYING;

	if( x1 == x2 && y1 == y2 ) {
		return;
	} else if( x1 == x2 ) {
		for( i = y1; dy_dir * i <= dy_dir * y2; i += dy_dir ) {
			ret = Collision_callback( obj, col, x1, floorf( i ), side, status, dy_dir * ( floorf( i ) - y1 ) );
			if( ret & STATUS_ALREADY_BLOCKED )
				status |= STATUS_ALREADY_BLOCKED;
			if( ret & STATUS_WALL )
				status &= ~STATUS_WALL;
		}
	} else if( y1 == y2 ) {
		for( j = x1; dx_dir * j <= dx_dir * x2; j += dx_dir ) {
			ret = Collision_callback( obj, col, floorf( j ), y1, side, status, 0 );
			if( ret & STATUS_ALREADY_BLOCKED )
				status |= STATUS_ALREADY_BLOCKED;
			if( ret & STATUS_WALL )
				status &= ~STATUS_WALL;
		}
	} else {
		m = ( y2 - y1 ) / ( x2 - x1 );

		for( j = x1; dx_dir * j <= dx_dir * x2; j += dx_dir ) {
			i = m * j;

			if( dy_dir * i <= dy_dir * y2 ) {
				ret = Collision_callback( obj, col, floorf( j ), floorf( i ), side, status, dy_dir * ( floorf( i ) - y1 ) );
				if( ret & STATUS_ALREADY_BLOCKED )
					status |= STATUS_ALREADY_BLOCKED;
				if( ret & STATUS_WALL )
					status &= ~STATUS_WALL;
			}
		}
	}

	if( ( status & STATUS_IS_SIDE_OF_WALKING ) && ( status & STATUS_WALL ) ) {
		obj->v.x = 0;
		obj->force.x = 0;
		obj->direction ^= 1;
		//printf("Has been blocked because of wall!\n");
	} else if( !( status & STATUS_ALREADY_BLOCKED ) && obj->propcp.y > 0 ) {
		obj->cp.y -= obj->propcp.y;
		//printf("Will go up of %d!\n", obj->propcp.y);
		obj->propcp.y = 0;
	}

	//if( status & STATUS_ALREADY_BLOCKED )
		//printf("Should be blocked!\n");
}

void Collision_detect( Object *obj, BITMAP *bmp ) {
	/*  p2 p3
	 *  +---+
	 *  |   |  cp
	 *  | x |_ angle
	 *  |   | |
	 *  +---+----
	 *  p1 p4
	 */
	//printf("-> Before P4->P3\n");
	Collision_trackLine( obj, bmp, floorf( obj->p[ P_DOWN_LEFT ].x ), floorf( obj->p[ P_DOWN_LEFT ].y ) + 1, floorf( obj->p[ P_DOWN_RIGHT ].x ), floor( obj->p[ P_DOWN_RIGHT ].y ) + 1, SIDE_DOWN );

	//printf("-> Before P4->P1\n");
	Collision_trackLine( obj, bmp, floorf( obj->p[ P_DOWN_LEFT ].x ) - 1, floorf( obj->p[ P_DOWN_LEFT ].y ), floorf( obj->p[ P_UP_LEFT ].x ) - 1, floor( obj->p[ P_UP_LEFT ].y ), SIDE_LEFT );

	//printf("-> Before P3->P2\n");
	Collision_trackLine( obj, bmp, floorf( obj->p[ P_DOWN_RIGHT ].x ) + 1, floorf( obj->p[ P_DOWN_RIGHT ].y ), floorf( obj->p[ P_UP_RIGHT ].x ) + 1, floor( obj->p[ P_UP_RIGHT ].y ), SIDE_RIGHT );

	//printf("-> Before P1->P2\n");
	Collision_trackLine( obj, bmp, floorf( obj->p[ P_UP_LEFT ].x ), floorf( obj->p[ P_UP_LEFT ].y ) - 1, floorf( obj->p[ P_UP_RIGHT ].x ), floor( obj->p[ P_UP_RIGHT ].y ) - 1, SIDE_UP );
}

int Collision_collisionCallback( Level* level, Object *obj, int x, int y ) {
	//putpixel( screen, x, y, makecol( 0, 0, 255 ) );
	//printf( "->Test %d,%d\n", x, y );

	if( getpixel( level->bmps.col, x, y ) == COLOR_WALL ) {
		//printf( "-> !! Collision detected at %d,%d !!\n", x, y );
		//obj->cp.x = x - obj->dir.x * obj->size.x / 2 - obj->dir.x;
		if( obj->delta.x < -X_SIGNIFICANT || obj->delta.x > X_SIGNIFICANT ) {
			//printf( "  -> Significant movement following X\n" );
			obj->cp.x = x - obj->dir.x * obj->size.x - obj->dir.x;
		}
		//obj->cp.y = y - obj->dir.y * obj->size.y / 2 - obj->dir.y;
		if( obj->delta.y < -Y_SIGNIFICANT || obj->delta.y > Y_SIGNIFICANT ) {
			//printf( "  -> Significant movement following Y\n" );
			obj->cp.y = y - obj->dir.y * obj->size.y - obj->dir.y;
		}
		return 1;
	}

	return 0;
}

void Collision_continuous( Level* level, Object *obj ) {
	//printf( "-> Start: %d, %d\n", (int) floorf( obj->oldcp.x ), (int) floorf( obj->oldcp.y ) );
	TrackLine( level, obj, floorf( obj->oldcp.x ), floorf( obj->oldcp.y ), floorf( obj->cp.x ), floorf( obj->cp.y ) + ( obj->size.y - 2 ), Collision_collisionCallback );
	//TrackLine( obj, col, floorf( obj->p[ P_UP_LEFT ].x + 2 ), floorf( obj->p[ P_UP_LEFT ].y + 2 ), floorf( obj->p[ P_DOWN_RIGHT ].x - 2 ), floorf( obj->p[ P_DOWN_RIGHT ].y - 2 ), Collision_collisionCallback );
	//printf( "-> End: %d, %d\n", (int) floorf( obj->cp.x ), (int) floorf( obj->cp.y ) );
}