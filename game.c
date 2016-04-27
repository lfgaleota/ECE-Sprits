#include "inc/game.h"

void Game_show( Level* level ) {
	ObjectM *maillon;

	clear_bitmap( level->bmps.page );

	clear_bitmap( level->bmps.stick_col );

	set_trans_blender( 255, 0, 255, 0 );

	Game_showBackground( level );

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

	blit( level->bmps.page, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H );
}

void Game_showBackground( Level* level ) {
	blit( level->bmps.back, level->bmps.page, 0, 0, 0, 0, level->bmps.back->w, level->bmps.back->h );
	masked_blit( level->bmps.col, level->bmps.page, 0, 0, 0, 0, level->bmps.col->w, level->bmps.col->h );
}

void Game_showForeground( Level* level ) {
	if( level->bmps.fore )
		masked_blit( level->bmps.fore, level->bmps.page, 0, 0, 0, 0, level->bmps.col->w, level->bmps.col->h );
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
		case STATE_FALLING:
			current = &level->bmps.stickmen_falling;
			break;

		case STATE_WALKING:
		default:
			current = &level->bmps.stickmen_walking;
			break;
	}

	if ( obj->counter >= current->count )
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

void Game_update( Level* level ) {
	ObjectM *maillon, *next;

	Game_updateLevelProperties( level );

	// On parcours les maillons
	for( maillon = level->stickmen; maillon != NULL; maillon = next ) {
		next = maillon->next;

		if( maillon->obj ) {
			if( maillon->obj->state != STATE_DEAD ) {
				Game_updateObjectProperties( level, maillon->obj );

				Physic_initMovement( maillon->obj, level->gravity, level->movement );

				Collision_detect( maillon->obj, level->bmps.col );

				Physic_compute( maillon->obj, level->dt );

				Collision_continuous( maillon->obj, level->bmps.col );
			} else {
				level->stickmen = Object_remove( level->stickmen, maillon->obj );
				level->nb_stickmen_dead++;
			}
		}
	}
}

void Game_launch( Level* level ) {
	level->dt = level->slow_dt;

	level->counter_stickmen_arrival = 0;
	level->nb_stickmen_added = 0;
	level->nb_stickmen_arrived = 0;
	level->nb_stickmen_dead = 0;

	level->win = 0;
	level->quit = 0;

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