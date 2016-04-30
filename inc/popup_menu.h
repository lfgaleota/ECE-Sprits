#ifndef _ECE_SPRITS_POPUP_MENU_H_
#define _ECE_SPRITS_POPUP_MENU_H_

#include <stdio.h>
#include <allegro.h>
#include <loadpng.h>

#define MENU_BACK_OFFSET 4
#define MENU_TOP_BAR_OFFSET 1
#define MENU_LEFT_BAR_OFFSET 1
#define MENU_FONT_SIZE 14
#define MENU_LETTER_SPACING_X 2
#define MENU_LETTER_SPACING_Y 5
#define MENU_PADDING_Y 20
#define MENU_PADDING_X 25

BITMAP* Menu_generateBackground( int width, int height );
void Menu_open( FONT* fonttext, char** choices, int count );

#endif //_ECE_SPRITS_POPUP_MENU_H_
