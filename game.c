#include "inc/game.h"

void Game_show( Level* level ) {
	ObjectM *maillon;

	clear_bitmap( level->bmps.page );

	clear_bitmap( level->bmps.stick_col );

	set_trans_blender( 255, 0, 255, 0 );

	Game_showBackground( level );

	draw_sprite( level->bmps.page, level->bmps.start.bmps[ 0 ], level->start.x - level->bmps.start.bmps[ 0 ]->w / 2, level->start.y - level->bmps.start.bmps[ 0 ]->h / 2 );
	draw_sprite( level->bmps.page, level->bmps.exit.bmps[ 0 ], level->exit.x - level->bmps.exit.bmps[ 0 ]->w / 2, level->exit.y - level->bmps.exit.bmps[ 0 ]->h / 2 );

	Game_showForeground( level );

	set_alpha_blender();

	// On parcours les maillons
	for( maillon = level->stickmen; maillon != NULL; maillon = maillon->next ) {
		if( maillon->obj ) {
			if( maillon->obj->direction )
				draw_trans_sprite( level->bmps.page, maillon->obj->bmp, maillon->obj->p[ P_UP_LEFT ].x, maillon->obj->p[ P_UP_LEFT ].y );
			else
				draw_sprite_h_flip( level->bmps.page, maillon->obj->bmp, maillon->obj->p[ P_UP_LEFT ].x, maillon->obj->p[ P_UP_LEFT ].y );

			rectfill( level->bmps.stick_col, maillon->obj->p[ P_UP_LEFT ].x, maillon->obj->p[ P_UP_LEFT ].y, maillon->obj->p[ P_DOWN_RIGHT ].x, maillon->obj->p[ P_DOWN_RIGHT ].y, maillon->obj->id );
		}
	}

	CircularMenu_show( level->capacities_menu, level->bmps.page, mouse_x, mouse_y );

	blit( level->bmps.page, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H );
}

void Game_showBackground( Level* level ) {
	blit( level->bmps.back, level->bmps.page, 0, 0, 0, 0, level->bmps.back->w, level->bmps.back->h );
}

void Game_showForeground( Level* level ) {
	if( level->bmps.fore ) {
		masked_blit( level->bmps.fore, level->bmps.page, 0, 0, 0, 0, level->bmps.col->w, level->bmps.col->h );
	}
}

void Game_updateLevelProperties( Level* level ) {
	Game_addStickmen( level );

	Game_checkWin( level );
}

void Game_checkWin( Level* level ) {
	if( level->nb_stickmen_arrived >= level->nb_stickmen_should_arrive ) {
		level->win = 1;
	}
}

void Game_updateObjectProperties( Level* level, Object* obj ) {
	Frames* current;

	if( obj->v.y < -Y_SIGNIFICANT || obj->v.y > Y_SIGNIFICANT ) {
		obj->state = STATE_FALLING;
	} else {
		obj->state = STATE_WALKING;
	}

	obj->counter++;
	switch( obj->state ) {

		case STATE_FALLDYING:
			current = &level->bmps.stickmen_falldying;
			if( obj->counter >= current->count )
				obj->state = STATE_DEAD;
			break;

		case STATE_DYING:
			current = &level->bmps.stickmen_dying;
			if( obj->counter >= current->count )
				obj->state = STATE_DEAD;
			break;

		case STATE_EXITING:
		case STATE_FALLING:
			current = &level->bmps.stickmen_falling;
			break;

		case STATE_WALKING:
		default:
			current = &level->bmps.stickmen_walking;
			break;
	}

	if( obj->counter >= current->count )
		obj->counter = 0;

	obj->bmp = current->bmps[ obj->counter ];
	obj->size.x = obj->bmp->h;
	obj->size.y = obj->bmp->w;
}

void Game_addStickmen( Level* level ) {
	Object* obj;

	if( level->nb_stickmen_added < level->nb_stickmen_max ) {
		level->counter_stickmen_arrival++;

		if( level->counter_stickmen_arrival > level->counter_stickmen_arrival_max ) {
			obj = Object_allocate();

			if( obj ) {
				Object_init( obj );

				obj->cp.x = level->start.x;
				obj->cp.y = level->start.y;

				Physic_computeDelta( obj );

				level->stickmen = Object_add( level->stickmen, obj, 1 );

				if( level->stickmen == NULL ) {
					allegro_message( "Erreur d'allocation du stickman" );
				} else {
					level->nb_stickmen_added++;
				}
			} else {
				allegro_message( "Erreur d'allocation du stickman" );
			}

			level->counter_stickmen_arrival = 0;
		}
	}
}

void Game_updateInputs( Level* level ) {
	level->inputs.prev_mouse_l = level->inputs.mouse_l;
	level->inputs.mouse_l = mouse_b & 1;
}

void Game_handleInputs( Level* level, Object* obj ) {
	ObjectM *maillon;
	char ret = 0;

	if( !level->inputs.prev_mouse_l && level->inputs.mouse_l ) {
		if( obj->selected == 1 ) {
			ret = CircularMenu_handleClick( level->capacities_menu, level, obj, mouse_x, mouse_y );

			if( ret == 1 ) {
				level->capacities_menu->opened = 0;
				obj->selected = 0;
			}
		} else {
			if( getpixel( level->bmps.stick_col, mouse_x, mouse_y ) == obj->id ) {
				for( maillon = level->stickmen; maillon != NULL; maillon = maillon->next ) {
					if( maillon->obj ) {
						maillon->obj->selected = 0;
					}
				}

				obj->selected = 1;
				level->capacities_menu->opened = 1;

			} else {
				obj->selected = 0;
			}
		}
	}
}

void Game_update( Level* level ) {
	ObjectM *maillon, *next;

	Game_updateInputs( level );
	Game_updateLevelProperties( level );

	// On parcours les maillons
	for( maillon = level->stickmen; maillon != NULL; maillon = next ) {
		next = maillon->next;

		if( maillon->obj ) {
			if( maillon->obj->state != STATE_DEAD ) {
				Game_handleInputs( level, maillon->obj );

				Game_updateObjectProperties( level, maillon->obj );

				Capacities_update( level, maillon->obj );

				Physic_initMovement( maillon->obj, level->gravity, level->movement );

				Collision_detect( maillon->obj, level->bmps.col );

				Physic_compute( maillon->obj, level->dt );

				Collision_continuous( level, maillon->obj );
			} else {
				level->stickmen = Object_remove( level->stickmen, maillon->obj );
				level->nb_stickmen_dead++;
			}
		}
	}
}

char Game_createMenus( Level* level ) {
	int i;

	MenuItem* capacities = malloc( sizeof( MenuItem ) * 4 );
	if( !capacities )
		return 0;

	MenuItem* directions = malloc( sizeof( MenuItem ) * 9 );
	if( !directions )
		return 0;

	for( i = 0; i < 4; i++ ) {
		capacities[ i ].bg_color = makecol( 254 - i, 254 - i, 254 - i );
		capacities[ i ].bg_sel_color = makecol( 200, 200, 200 );
		capacities[ i ].text_color = makecol( 0, 0, 0 );
		capacities[ i ].callback = Capacities_set;
		capacities[ i ].tooltip = malloc( sizeof( char ) * 40 );

		if( !capacities[ i ].tooltip )
			return 0;
	}
	capacities[ CAPACITY_BUILD ].icon = level->bmps.capacity_build;
	sprintf( capacities[ CAPACITY_BUILD ].tooltip, "Construire (reste %d)", level->capacities.building );
	capacities[ CAPACITY_DIG ].icon = level->bmps.capacity_dig;
	sprintf( capacities[ CAPACITY_DIG ].tooltip, "Creuser (reste %d)", level->capacities.building );
	capacities[ CAPACITY_BLOW ].icon = level->bmps.capacity_blow;
	sprintf( capacities[ CAPACITY_BLOW ].tooltip, "Souffler (reste %d)", level->capacities.building );
	capacities[ 3 ].icon = level->bmps.arrow[ 8 ];
	strcpy( capacities[ 3 ].tooltip, "Retour" );

	for( i = 0; i < 9; i++ ) {
		directions[ i ].icon = level->bmps.arrow[ i ];
		directions[ i ].bg_color = makecol( 254 - i, 254 - i, 254 - i );
		directions[ i ].bg_sel_color = makecol( 200, 200, 200 );
		directions[ i ].text_color = makecol( 0, 0, 0 );
		directions[ i ].callback = Capacities_setDirectionCallback;
		directions[ i ].tooltip = malloc( sizeof( char ) * 40 );
		strcpy( directions[ i ].tooltip, "" );
	}
	strcpy( directions[ 8 ].tooltip, "Retour" );

	level->capacities_menu = CircularMenu_create( 0, 0, 40, 40, capacities, 4 );
	level->directions_menu = CircularMenu_create( 0, 0, 40, 40, directions, 8 );

	CircularMenu_compute( level->capacities_menu );
	CircularMenu_compute( level->directions_menu );

	return 1;
}

char Game_createBitmaps( Level* level ) {
	level->bmps.page = create_bitmap( 1024, 768 );
	if( !level->bmps.page ) {
		allegro_message( "Erreur creation bitmap" );
		return 0;
	}

	level->bmps.stick_col = create_bitmap( 1024, 768 );
	if( !level->bmps.stick_col ) {
		allegro_message( "Erreur creation bitmap" );
		return 0;
	}

	return 1;
}

void Game_generateTexture( BITMAP* col, BITMAP* texture, BITMAP* dest, int color ) {
	unsigned int i, j;

	for( i = 0; i < col->h; i++ ) {
		for( j = 0; j < col->w; j++ ) {
			if( getpixel( col, j, i ) == color ) {
				putpixel( dest, j, i, getpixel( texture, j % texture->w, i % texture->h ) );
			}
		}
	}
}

void Game_generateTextures( Level* level ) {
	BITMAP* temp = create_bitmap( level->bmps.col->w, level->bmps.col->h );

	if( temp ) {
		rectfill( temp, 0, 0, temp->w, temp->h, makecol( 255, 0, 255 ) );

		if( level->bmps.wall ) {
			Game_generateTexture( level->bmps.col, level->bmps.wall, temp, COLOR_WALL );
		}

		if( level->bmps.deathzone ) {
			Game_generateTexture( level->bmps.col, level->bmps.deathzone, temp, COLOR_DEATHZONE );
		}

		set_trans_blender( 255, 0, 255, 0 );
		masked_blit( level->bmps.fore, temp, 0, 0, 0, 0, temp->w, temp->h );
		masked_blit( temp, level->bmps.fore, 0, 0, 0, 0, temp->w, temp->h );

		destroy_bitmap( temp );
	}
}

char Game_levelInit( Level* level ) {
	char ret = 0;

	level->stickmen = NULL;

	level->dt = level->slow_dt;

	level->counter_stickmen_arrival = 0;
	level->nb_stickmen_added = 0;
	level->nb_stickmen_arrived = 0;
	level->nb_stickmen_dead = 0;

	level->win = 0;
	level->quit = 0;

	Game_generateTextures( level );

	ret = Game_createMenus( level );

	ret &= Game_createBitmaps( level );

	return ret;
}

void Game_launch( Level* level ) {
	if( !Game_levelInit( level ) ) {
		allegro_message( "Impossible de lancer le niveau!" );
		return;
	}

	while( !level->quit ) {

		if( key[ KEY_SPACE ] ) {
			if( level->dt == level->slow_dt ) {
				level->dt = level->fast_dt;
			} else if( level->dt == level->fast_dt ) {
				level->dt = 0;
			} else {
				level->dt = level->slow_dt;
			}
		}

		Game_update( level );

		Game_show( level );

		SLEEP( 1 / 30.0 * 1000 ); // 30 FPS idéalement
	}
}
