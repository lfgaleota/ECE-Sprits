#include "inc/menu.h"

void Menu_newGame( FMod* fmod, unsigned char id ) {
	Save save;
	save = Save_load( id );
	save.standard_level = LEVEL_MINSTANDARD;
	if( !Save_save( &save ) )
		allegro_message( "Erreur lors de la sauvegarde!" );

	Menu_launchGame( fmod, save.id );
}

void Menu_launchGame( FMod* fmod, unsigned char id ) {
	Save save;
	Level* level;
	char quit = 0;

	save = Save_load( id );

	if( ( save.standard_level >= LEVEL_MINSTANDARD ) && ( save.standard_level <= LEVEL_MAXSTANDARD ) ) {
		// On lance la boucle des niveaux tnt qu'il ne faut pas quitter
		while( !quit ) {
			// On recupère le niveau standard désigné par la sauvegarde
			level = Level_loadStandard( save.standard_level );

			// On lance le niveau s'il a été chargé
			if( level ) {
				level->fmod = fmod;
				// On fait tourner le niveau et récupère la sortie
				switch( Game_launch( level ) ) {
					case QUIT_WIN: // S'il a réussi le niveau
						save.standard_level++; // On passe au niveau suivant au niveau de la sauvegarde
						// On sauvegarde
						if( !Save_save( &save ) )
							allegro_message( "Erreur lors de la sauvegarde!" );
						break;

					case QUIT_DELIBERATE: // S'il souhaite quitter
						// On quitte la boucle
						quit = 1;
						break;

					default: // Sinon on relance le niveau
						break;
				}

				// On libère la mémoire
				Level_free( level );
			} else {
				// On informe le joueur
				allegro_message( "Impossible de charger le niveau %d! Le jeu va retourner au menu.", save.standard_level );

				// On quitte la boucle
				quit = 1;
			}
		}
	}
}

void Menu_showBackground( Menu* menu ) {
	clear_bitmap( menu->page );

	if( menu->submenu == MENU_MAIN ) {
		blit( menu->back, menu->page, menu->back->w - SCREEN_W, 0, 0, 0, SCREEN_W, SCREEN_H );
	} else {
		blit( menu->back, menu->page, 0, 0, 0, 0, SCREEN_W, SCREEN_H );
	}
}

void Menu_showForeground( Menu* menu ) {
	int colors[ 6 ];
	int px;
	int i;

	set_alpha_blender();
	
	// Attribution des couleurs de la carte de collision pour realiser les tests
	for( i = 0; i < 6; i++ ) {
		colors[ i ] = makecol( 50 * i, 30 * i, 3 * i );
	}

	if( menu->submenu == MENU_MAIN ) {
		// Récupération de la couleur
		px = getpixel( menu->col1, mouse_x, mouse_y );

		// Tests successifs des couleurs puis affichage à l'ecran d'un nouveau avant-plan et sélection du choix
		if( px == colors[ 1 ] ) {
			draw_trans_sprite( menu->page, menu->fore1.bmps[ 1 ], 0, 0 );
			menu->choice = 1;
		} else if( px == colors[ 2 ] ) {
			draw_trans_sprite( menu->page, menu->fore1.bmps[ 2 ], 0, 0 );
			menu->choice = 2;
		} else if( px == colors[ 3 ] ) {
			draw_trans_sprite( menu->page, menu->fore1.bmps[ 3 ], 0, 0 );
			menu->choice = 3;
		} else if( px == colors[ 4 ] ) {
			draw_trans_sprite( menu->page, menu->fore1.bmps[ 4 ], 0, 0 );
			menu->choice = 4;
		} else if( px == colors[ 5 ] ) {
			draw_trans_sprite( menu->page, menu->fore1.bmps[ 5 ], 0, 0 );
			menu->choice = 5;
		} else {
			draw_trans_sprite( menu->page, menu->fore1.bmps[ 0 ], 0, 0 );
			menu->choice = 0;
		}
	} else {
		// Récupération de la couleur
		px = getpixel( menu->col2, mouse_x, mouse_y );

		// Tests successifs des couleurs puis affichage à l'ecran d'un nouveau avant-plan
		if( px == colors[ 1 ] ) {
			draw_trans_sprite( menu->page, menu->fore2.bmps[ 1 ], 0, 0 );
			menu->choice = 1;
		} else if( px == colors[ 2 ] ) {
			draw_trans_sprite( menu->page, menu->fore2.bmps[ 2 ], 0, 0 );
			menu->choice = 2;
		} else if( px == colors[ 3 ] ) {
			draw_trans_sprite( menu->page, menu->fore2.bmps[ 3 ], 0, 0 );
			menu->choice = 3;
		} else if( px == colors[ 4 ] ) {
			draw_trans_sprite( menu->page, menu->fore2.bmps[ 4 ], 0, 0 );
			menu->choice = 4;
		} else {
			draw_trans_sprite( menu->page, menu->fore2.bmps[ 0 ], 0, 0 );
			menu->choice = 0;
		}
	}
}

void Menu_show( Menu* menu ) {
	Menu_showBackground( menu );
	Menu_showForeground( menu );
	blit( menu->page, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H );
}

void Menu_transition( Menu* menu, unsigned char direction ) {
	/*int i;

	if( direction == MENU_MAIN ) {
		for( i = 0; i < menu->back->w - SCREEN_W; i++ ) {
			blit( menu->back, screen, i, 0, 0, 0, SCREEN_W, SCREEN_H );

			SLEEP( 1/30 * 1000 );
		}
	} else {
		for( i = menu->back->w - SCREEN_W; i > 0; i-- ) {
			blit( menu->back, screen, i, 0, 0, 0, SCREEN_W, SCREEN_H );

			SLEEP( 1/30 * 1000 );
		}
	}*/
}

char Menu_load( Menu* menu ) {
	//Creation et chargement des bitmaps
	menu->page = create_bitmap( SCREEN_W, SCREEN_H );
	if( !menu->page ) {
		allegro_message( "Problème d'allocation de bitmap." );
		return 0;
	}

	menu->back = load_jpg( "images/menu/fond.jpg", NULL );
	menu->col1 = load_png( "images/menu/collision1.png", NULL );
	menu->col2 = load_png( "images/menu/collision2.png", NULL );
	menu->fore1 = Level_loadFrames( "images/menu/fond1sel", 6 );
	menu->fore2 = Level_loadFrames( "images/menu/fond2sel", 5 );

	if( !menu->back || !menu->fore1.bmps || !menu->fore2.bmps || !menu->col1 || !menu->col2 ) {
		allegro_message( "Impossible de charger les images." );
		return 0;
	}

	menu->submenu = MENU_MAIN;
	menu->choice = 0;
	
	return 1;
}

void Menu_free( Menu* menu ) {
	Level_freeBitmap( menu->page );
	Level_freeBitmap( menu->back );
	Level_freeBitmap( menu->col1 );
	Level_freeBitmap( menu->col2 );
	Level_freeFrames( &menu->fore1 );
	Level_freeFrames( &menu->fore2 );
}

void Menu_options( Menu* menu, GeneralConfig* config ) {
	PopupMenu_alert( "Attention, en l'etat actuel, ces options peuvent causer des problemes.", "J'accepte", NULL );
	Menu_show( menu );

	gfx_mode_select_ex( &config->card, &config->width, &config->height, &config->color_depth );
	GeneralConfig_save( config );

	set_color_depth( config->color_depth );
	if( set_gfx_mode( config->card, config->width, config->height, 0, 0 ) != 0 ) {
		allegro_message( "Problème mode graphique" );
		allegro_exit();
		exit( EXIT_FAILURE );
	}

	enable_hardware_cursor();
	select_mouse_cursor( 2 );
	show_mouse( screen );
}

void Menu_rules( Menu* menu ) {

}

void Menu_launch( FMod* fmod, GeneralConfig* config ) {
	Menu menu;

	if( !Menu_load( &menu ) )
		return;

	int quit = 0, prev_mouse_l, mouse_l = 0;

	if( !fmod )
		return;

	menu.fmod = fmod;

	//Boucle de jeu
	while( !quit ) {
		prev_mouse_l = mouse_l;
		mouse_l = mouse_b & 1;

		Menu_show( &menu );

		if( !prev_mouse_l && mouse_l ) {
			switch( menu.choice ) {
				case 1:
					if( menu.submenu == MENU_MAIN ) {
						// Passage au menu de nouvelle partie
						menu.submenu = MENU_NEW;
						Menu_transition( &menu, MENU_NEW );
					} else if( menu.submenu == MENU_NEW ) {
						// Créer une partie sur la sauvegarde 1
						Menu_newGame( fmod, 1 );
						menu.submenu = MENU_MAIN;
					} else if( menu.submenu == MENU_LOAD ) {
						// Charger une partie sur la sauvegarde 1
						Menu_launchGame( fmod, 1 );
						menu.submenu = MENU_MAIN;
					}
					break;

				case 2:
					if( menu.submenu == MENU_MAIN ) {
						// Passage au menu de chargement de partie
						menu.submenu = MENU_LOAD;
						Menu_transition( &menu, MENU_NEW );
					} else if( menu.submenu == MENU_NEW ) {
						// Créer une partie sur la sauvegarde 2
						Menu_newGame( fmod, 2 );
						menu.submenu = MENU_MAIN;
					} else if( menu.submenu == MENU_LOAD ) {
						// Charger une partie sur la sauvegarde 2
						Menu_launchGame( fmod, 2 );
						menu.submenu = MENU_MAIN;
					}
					break;

				case 3:
					if( menu.submenu == MENU_MAIN ) {
						// Règles
						Menu_rules( &menu );
					} else if( menu.submenu == MENU_NEW ) {
						// Créer une partie sur la sauvegarde 3
						Menu_newGame( fmod, 3 );
						menu.submenu = MENU_MAIN;
					} else if( menu.submenu == MENU_LOAD ) {
						// Charger une partie sur la sauvegarde 3
						Menu_launchGame( fmod, 3 );
						menu.submenu = MENU_MAIN;
					}
					break;

				case 4:
					if( menu.submenu == MENU_MAIN ) {
						// Options
						Menu_options( &menu, config );
					} else {
						// Retour au menu principal
						menu.submenu = MENU_MAIN;
						Menu_transition( &menu, MENU_MAIN );
					}
					break;

				case 5:
					if( menu.submenu == MENU_MAIN ) {
						// Quitter
						quit = 1;
					}
					break;
			}
		}

		// Petite pause
		SLEEP( 20 );
	}

	Menu_free( &menu );
}