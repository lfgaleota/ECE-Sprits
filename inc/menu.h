#ifndef _ECE_SPRITS_MENU_H_
#define _ECE_SPRITS_MENU_H_

#include <allegro.h>
#include <loadpng.h>
#include <jpgalleg.h>
#include "structures.h"
#include "fmod.h"
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
	FMod* fmod;
};

void Menu_newGame( FMod* fmod, unsigned char id );
void Menu_launchGame( FMod* fmod, unsigned char id );

void Menu_showBackground( Menu* menu );
void Menu_showForeground( Menu* menu );
void Menu_show( Menu* menu );

void Menu_options( Menu* menu, GeneralConfig* config );
void Menu_rules( Menu* menu );

void Menu_transition( Menu* menu, unsigned char direction );

char Menu_load( Menu* menu );
void Menu_free( Menu* menu );

void Menu_launch( FMod* fmod, GeneralConfig* config );

#endif
