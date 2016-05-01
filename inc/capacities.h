#ifndef _ECE_SPRITS_CAPACITIES_H_
#define _ECE_SPRITS_CAPACITIES_H_

#include <stdio.h>
#include <allegro.h>
#include <math.h>
#include "structures.h"
#include "line_tracking.h"
#include "circular_menu.h"
#include "fmod.h"

void Capacities_update( Level* level, Object* obj );

char Capacities_setDirectionCallback( Level* level, Object* obj, int index, int x, int y );
char Capacities_set( Level* level, Object* obj, int index, int x, int y );
void Capacities_setDigging( Level* level, Object* obj );

#endif //_ECE_SPRITS_CAPACITIES_H_
