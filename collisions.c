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

	// Si le personnage n'est pas déjà bloqué
	if( !( status & STATUS_ALREADY_BLOCKED ) ) {
		// S'il se dirige vers le haut ou le bas, et qu'on étudie le côté suivant le mouvement
		if( ( side == SIDE_UP && obj->dir.y <= 0 ) || ( side == SIDE_DOWN && obj->dir.y >= 0 ) ) {
			// On bloque le personnage
			obj->v.y = 0;
			obj->force.y = 0;
			ret |= STATUS_ALREADY_BLOCKED;

			// Si sa vitesse est trop importante
			if( obj->v.y > Y_FALLDAMAGE ) {
				// On le tue
				obj->state = STATE_FALLDYING;
			}
		}

		// Si on étudie le côté gauche, qu'on se trouve sur un deuxième mur et qu'il se déplace
		if( side == SIDE_LEFT && !( status & STATUS_WALL ) && obj->v.x != 0 ) {
			// On le bloque
			obj->v.x = 0;
			obj->force.x = 0;
			obj->direction = DIRECTION_RIGHT;
			ret |= STATUS_ALREADY_BLOCKED;
		}

		// Si on étudie le côté droit, qu'on se trouve sur un deuxième mur et qu'il se déplace
		if( side == SIDE_RIGHT && !( status & STATUS_WALL ) && obj->v.x != 0 ) {
			// On le bloque
			obj->v.x = 0;
			obj->force.x = 0;
			obj->direction = DIRECTION_LEFT;
			ret |= STATUS_ALREADY_BLOCKED;
		}
	}

	// On calcule la vitesse projeté sur la normale au contact
	/*float v_along_normal = obj->v.x * normal.x + obj->v.y * normal.y;

	// Si l'objet s'éloigne on ne fait rien
	if( v_along_normal > 0 ) {
		return ret;
	}

	float j = -( 1 + obj->restitution ) * v_along_normal / obj->mass / obj->size.x;

	// On applique l'impulsion calculée
	Vector2 impulse = (Vector2) { normal.x * j, normal.y * j };

	Vector2 r = ( Vector2 ) { x - obj->cp.x, y - obj->cp.y };
	obj->torque = r.x * impulse.y - r.y * impulse.x;

	obj->force.x += impulse.x;
	obj->force.y += impulse.y;*/

	return ret;
}

void Collision_exitCallback( Object* obj, unsigned char status ) {
	// Si on se trouve selon le côté de déplacement vertical ou horizontal, et qu'il n'est pas déjà en train de sortir
	if( ( status & STATUS_IS_SIDE_OF_FLYING || status & STATUS_IS_SIDE_OF_WALKING ) && obj->state != STATE_EXITING ) {
		// On le fait sortir
		obj->state = STATE_EXITING;
		obj->counter = 0;
	}
}

void Collision_deathZoneCallback( Object* obj, unsigned char status ) {
	// Si on se trouve selon le côté de déplacement vertical ou horizontal, et qu'il n'est pas déjà en train de mourir
	if( ( status & STATUS_IS_SIDE_OF_FLYING || status & STATUS_IS_SIDE_OF_WALKING ) && obj->state != STATE_DYING ) {
		// On le tue
		obj->state = STATE_DYING;
		obj->counter = 0;
	}
}

void Collision_windCallback( Object* obj, int color, unsigned char status ) {
	float angle;

	// Si on se trouve selon le côté de déplacement vertical ou horizontal, et qu'il peut bouger
	if( ( status & STATUS_IS_SIDE_OF_FLYING || status & STATUS_IS_SIDE_OF_WALKING ) && obj->should_move && !( status & STATUS_ALREADY_BLOCKED ) ) {
		// On applique le vent en récupérant l'angle qu'il fait par rapport à l'horizontal (codée par la couleur)
		angle = ( color - 1 ) * ( 2 * M_PI ) / makecol( 254, 254, 254 );
		obj->force.x += WIND_FORCE_X * cosf( angle ) / obj->size.y;
		obj->force.y -= WIND_FORCE_Y * sinf( angle ) / obj->size.y;
	}
}

char Collision_callback( Object* obj, BITMAP* col, BITMAP* wind_col, int x, int y, char side, unsigned char status, int progression_y ) {
	// On récupère les pixels des cartes de collisions
	int px = getpixel( col, x, y ), px2 = getpixel( wind_col, x, y );
	char ret = 0;

	// Si le pixel est celui d'un mur
	if( px == COLOR_WALL || px == COLOR_BREAKABLEWALL ) {
		// On fait ce qui est nécessaire
		ret = Collision_wallCallback( obj, side, status );
	} else {
		// Sinon
		// On indique que le 1er mur est fini
		ret |= STATUS_WALL;

		// Si le pixel est celui de la sortie
		if( px == COLOR_EXIT ) {
			// On fait ce qui est nécessaire
			Collision_exitCallback( obj, status );
		} else if( px == COLOR_DEATHZONE ) { // Si le pixel est celui d'une zone mortelle
			// On fait ce qui est nécessaire
			Collision_deathZoneCallback( obj, status );
		} else if( px2 != COLOR_NEUTRAL ) { // Si on a un pixel non-neutre sur la carte des vents
			// On fait ce qui est nécessaire
			Collision_windCallback( obj, px2, status );
		}

		// Si on est sur le côté selon le déplacement horizontal, que le personnage se déplace et qu'on est sur le 2ème pixel dans l'odre des tests
		if( status & STATUS_IS_SIDE_OF_WALKING && progression_y > 0 && obj->dir.x != 0 ) {
			// Si on était sur un mur continu avant
			if( status & STATUS_WALL ) {
				// Si l'objet peut se déplacer et qu'il peut monter
				if( progression_y < MAX_STEP && obj->should_move ) {
					// On le fait monter
					obj->propcp.y += progression_y;
				} else {
					// Sinon on le bloque
					obj->v.x = 0;
					obj->force.x = 0;
					if( side == SIDE_LEFT )
						obj->direction = DIRECTION_RIGHT;
					else
						obj->direction = DIRECTION_LEFT;
					ret |= STATUS_ALREADY_BLOCKED;
				}
			}
		}
	}

	return ret;
}

void Collision_trackLine( Object* obj, BITMAP* col, BITMAP* wind_col, int x1, int y1, int x2, int y2, char side ) {
	int dx, dy, dx_dir, dy_dir;
	char ret;
	unsigned char status = 0x2;
	float i, j, m;

	// On calcule la différence de position, et on définie dx_dir, dy_dir tel qu'il représente le sens de déplacement selon ces 2 positions
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

	// Si on est selon le côté du déplacement horizontal, on l'indique
	if( ( side == SIDE_LEFT && obj->dir.x < 0 ) || ( side == SIDE_RIGHT && obj->dir.x > 0 ) )
		status |= STATUS_IS_SIDE_OF_WALKING;

	// Si on est selon le côté du déplacement vertical, on l'indique
	if( ( side == SIDE_UP && obj->dir.y < 0 ) || ( side == SIDE_DOWN && obj->dir.y > 0 ) )
		status |= STATUS_IS_SIDE_OF_FLYING;

	// Si les 2 positions sont égales, on quitte
	if( x1 == x2 && y1 == y2 ) {
		return;
	} else if( x1 == x2 ) { // Si les positions sont alignées verticalement, on parcours verticalement
		for( i = y1; dy_dir * i <= dy_dir * y2; i += dy_dir ) {
			// On analyse le pixel
			ret = Collision_callback( obj, col, wind_col, x1, floorf( i ), side, status, dy_dir * ( floorf( i ) - y1 ) );

			// S'il est retourné que l'objet est bloqué on l'indique, même chose pour le mur continu
			if( ret & STATUS_ALREADY_BLOCKED )
				status |= STATUS_ALREADY_BLOCKED;
			if( ret & STATUS_WALL )
				status &= ~STATUS_WALL;
		}
	} else if( y1 == y2 ) { // Si les positions sont alignées horizontalement, on parcours horizontalement
		for( j = x1; dx_dir * j <= dx_dir * x2; j += dx_dir ) {
			// On analyse le pixel
			ret = Collision_callback( obj, col, wind_col, floorf( j ), y1, side, status, 0 );

			// S'il est retourné que l'objet est bloqué on l'indique, même chose pour le mur continu
			if( ret & STATUS_ALREADY_BLOCKED )
				status |= STATUS_ALREADY_BLOCKED;
			if( ret & STATUS_WALL )
				status &= ~STATUS_WALL;
		}
	} else { // Sinon on calcule le coefficient directeur de la pente et on parcours la pente
		m = ( y2 - y1 ) / ( x2 - x1 );

		for( j = x1; dx_dir * j <= dx_dir * x2; j += dx_dir ) {
			i = m * j;

			if( dy_dir * i <= dy_dir * y2 ) {
				// On analyse le pixel
				ret = Collision_callback( obj, col, wind_col, floorf( j ), floorf( i ), side, status, dy_dir * ( floorf( i ) - y1 ) );

				// S'il est retourné que l'objet est bloqué on l'indique, même chose pour le mur continu
				if( ret & STATUS_ALREADY_BLOCKED )
					status |= STATUS_ALREADY_BLOCKED;
				if( ret & STATUS_WALL )
					status &= ~STATUS_WALL;
			}
		}
	}

	// Si on a un mur continu tout du long, et qu'on est selon le côté du déplacement horizontal
	if( ( status & STATUS_IS_SIDE_OF_WALKING ) && ( status & STATUS_WALL ) ) {
		// On bloque l'objet
		obj->v.x = 0;
		obj->force.x = 0;
		obj->direction ^= 1;
	} else if( !( status & STATUS_ALREADY_BLOCKED ) && obj->propcp.y > 0 ) { // S'il nest pas bloqué et qu'il peut monter sur un obstacle
		// On applique le déplacemetn vertical correspondant
		obj->cp.y -= obj->propcp.y;
		obj->propcp.y = 0;
	}

	// Si le personnage a été bloqué et qu'il est en train de creuser ou construire
	if( status & STATUS_ALREADY_BLOCKED && ( obj->state == STATE_DIGGING || obj->state == STATE_BUILDING ) ) {
		// On le remet dans un état normal
		obj->state = STATE_WALKING;
		obj->capacities.digging = 0;
		obj->capacities.building = 0;
	}
}

void Collision_detect( Object *obj, BITMAP *col, BITMAP* wind_col ) {
	/*  p2 p3
	 *  +---+
	 *  |   |  cp
	 *  | x |_ angle
	 *  |   | |
	 *  +---+----
	 *  p1 p4
	 */

	// On parcours les 4 côtés du personnages
	Collision_trackLine( obj, col, wind_col, floorf( obj->p[ P_DOWN_LEFT ].x ), floorf( obj->p[ P_DOWN_LEFT ].y ) + 1, floorf( obj->p[ P_DOWN_RIGHT ].x ), floor( obj->p[ P_DOWN_RIGHT ].y ) + 1, SIDE_DOWN );

	Collision_trackLine( obj, col, wind_col, floorf( obj->p[ P_DOWN_LEFT ].x ) - 1, floorf( obj->p[ P_DOWN_LEFT ].y ), floorf( obj->p[ P_UP_LEFT ].x ) - 1, floor( obj->p[ P_UP_LEFT ].y ), SIDE_LEFT );

	Collision_trackLine( obj, col, wind_col, floorf( obj->p[ P_DOWN_RIGHT ].x ) + 1, floorf( obj->p[ P_DOWN_RIGHT ].y ), floorf( obj->p[ P_UP_RIGHT ].x ) + 1, floor( obj->p[ P_UP_RIGHT ].y ), SIDE_RIGHT );

	Collision_trackLine( obj, col, wind_col, floorf( obj->p[ P_UP_LEFT ].x ), floorf( obj->p[ P_UP_LEFT ].y ) - 1, floorf( obj->p[ P_UP_RIGHT ].x ), floor( obj->p[ P_UP_RIGHT ].y ) - 1, SIDE_UP );
}

int Collision_collisionCallback( Level* level, Object *obj, int x, int y ) {
	int px = getpixel( level->bmps.col, x, y );

	// Si on rencontre un pixel de mur
	if( px == COLOR_WALL || px == COLOR_BREAKABLEWALL ) {
		// Si le déplacement selon l'axe des X est significatif
		if( obj->delta.x < -X_SIGNIFICANT || obj->delta.x > X_SIGNIFICANT ) {
			// On le ramène en arrière pour éviter qu'il se coince
			obj->cp.x = x - obj->dir.x * obj->size.x - obj->dir.x;
		}

		// Si le déplacement selon l'axe des Y est significatif
		if( obj->delta.y < -Y_SIGNIFICANT || obj->delta.y > Y_SIGNIFICANT ) {
			// On le ramène en arrière pour éviter qu'il se coince
			obj->cp.y = y - obj->dir.y * obj->size.y - obj->dir.y;
		}

		// On indique qu'on peut arrêter de suivre la ligne
		return 1;
	}

	// Sinon on indique qu'on continue
	return 0;
}

void Collision_continuous( Level* level, Object *obj ) {
	// On parcours la ligne entre l'ancienne position et la nouvelle position du personnage
	TrackLine( level, obj, floorf( obj->oldcp.x ), floorf( obj->oldcp.y ), floorf( obj->cp.x ), floorf( obj->cp.y ) + ( obj->size.y - 2 ), Collision_collisionCallback );
}