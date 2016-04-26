#ifndef _ECE_SPRITS_PHYSICS_H_
#define _ECE_SPRITS_PHYSICS_H_

#include <math.h>
#include "structures.h"

float Vector_norm( Vector2 vec );

void Physic_computeDelta( Object* obj );
void Physic_updatePosition( Object* obj );
void Physic_compute( Object *obj, double dt );
void Physic_initMovement( Object *obj, Vector2 gravity, Vector2 movement );

#endif //_ECE_SPRITS_PHYSICS_H_
