#ifndef _ECE_SPRITS_MENU_H_
#define _ECE_SPRITS_MENU_H_

#include <allegro.h>
#include <loadpng.h>
#include <jpgalleg.h>
#include "structures.h"
#include "level.h"
#include "game.h"

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

void Menu_launch();

#endif
