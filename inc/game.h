#ifndef _ECE_SPRITS_GAME_H_
#define _ECE_SPRITS_GAME_H_

#include <allegro.h>
#include <math.h>
#include "structures.h"
#include "objects.h"
#include "physics.h"
#include "collisions.h"

void Game_launch( Level* level );
void Game_show( Level* level );

#endif //_ECE_SPRITS_GAME_H_
