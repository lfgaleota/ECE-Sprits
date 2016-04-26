#ifndef _ECE_SPRITS_CIRCULAR_MENU_H_
#define _ECE_SPRITS_CIRCULAR_MENU_H_

#include <allegro.h>
#include <math.h>

typedef struct MenuItem MenuItem;

struct MenuItem {
	BITMAP *icon;
	char *tooltip;
	int bg_color;
	int bg_sel_color;
	int text_color;

	int ( *callback )( int index, int x, int y );

	int center_x;
	int center_y;
	int icon_x;
	int icon_y;
};

typedef struct CircularMenu CircularMenu;
struct CircularMenu {
	char opened;
	int count;
	int inner_radius;
	int medium_radius;
	int outer_radius;
	float angle;
	int screen_x;
	int screen_y;
	BITMAP *bmp;
	MenuItem *items;
};

#endif //_ECE_SPRITS_CIRCULAR_MENU_H_
