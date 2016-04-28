#ifndef _ECE_SPRITS_CIRCULAR_MENU_H_
#define _ECE_SPRITS_CIRCULAR_MENU_H_

#include <allegro.h>
#include <math.h>
#include "structures.h"

CircularMenu *CircularMenu_create( int x, int y, int radius, int size, MenuItem *items, int count );
void CircularMenu_compute( CircularMenu *menu );
void CircularMenu_recompute( CircularMenu *menu );
void CircularMenu_updatePosition( CircularMenu* menu, int x, int y );
void CircularMenu_show( CircularMenu* menu, BITMAP* dest, int x, int y );
char CircularMenu_handleClick( CircularMenu* menu, Level* level, Object* obj, int x, int y );
void CircularMenu_free( CircularMenu* menu, char free_items, char free_items_bitmaps, char free_items_tooltips );

#endif //_ECE_SPRITS_CIRCULAR_MENU_H_
