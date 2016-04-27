#ifndef _ECE_SPRITS_COLLISIONS_H_
#define _ECE_SPRITS_COLLISIONS_H_

#include <allegro.h>
#include <math.h>
#include <stdio.h>
#include "structures.h"
#include "line_tracking.h"

void Collision_detect( Object *obj, BITMAP *bmp );
void Collision_trackLine( Object* obj, BITMAP* col, int x1, int y1, int x2, int y2, char side );
char Collision_callback( Object* obj, BITMAP* col, int x, int y, char side, unsigned char status, int progression_y );

void Collision_continuous( Level* level, Object *obj );
int Collision_collisionCallback( Level* level, Object *obj, int x, int y );

#endif //_ECE_SPRITS_COLLISIONS_H_
