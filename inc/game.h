#ifndef _ECE_SPRITS_GAME_H_
#define _ECE_SPRITS_GAME_H_

#include <allegro.h>
#include <fblend.h>
#include <math.h>
#include "structures.h"
#include "objects.h"
#include "physics.h"
#include "collisions.h"
#include "capacities.h"
#include "popup_menu.h"

void Game_showBackground( Level* level );
void Game_showForeground( Level* level );
void Game_show( Level* level );

void Game_updateLevelProperties( Level* level );
void Game_checkWin( Level* level );
void Game_updateObjectProperties( Level* level, Object* obj );
void Game_addStickmen( Level* level );

char Game_launch( Level* level );

#endif //_ECE_SPRITS_GAME_H_
