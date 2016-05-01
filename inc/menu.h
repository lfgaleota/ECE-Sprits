#ifndef _ECE_SPRITS_MENU_H_
#define _ECE_SPRITS_MENU_H_

#include <allegro.h>
#include <loadpng.h>
#include <jpgalleg.h>
#include "structures.h"
#include "level.h"
#include "game.h"
#include "save.h"

#define MENU_MAIN 1
#define MENU_NEW 2
#define MENU_LOAD 3

typedef struct Menu Menu;
struct Menu {
	unsigned char submenu;
	int choice;
	BITMAP* page;
	BITMAP* back;
	BITMAP* col1;
	BITMAP* col2;
	Frames fore1;
	Frames fore2;
};

void Menu_newGame( unsigned char id );
void Menu_launchGame( unsigned char id );

void Menu_showBackground( Menu* menu );
void Menu_showForeground( Menu* menu );

void Menu_transition( Menu* menu, unsigned char direction );

char Menu_load( Menu* menu );
void Menu_free( Menu* menu );

void Menu_launch( GeneralConfig* config );

#endif
