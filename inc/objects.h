#ifndef _ECE_SPRITS_OBJECTS_H_
#define _ECE_SPRITS_OBJECTS_H_

#include <allegro.h>
#include "structures.h"
#include "memory.h"

void Object_initMovment( Object *obj );
void Object_updatePosition( Object* obj );
void Object_init( Object *obj );
void Object_computeMomentOfInertia( Object* obj );

int Object_getUID( ObjectM* first );
ObjectM* Object_findMaillon( ObjectM* first, int id );
int Object_add( ObjectM* first, Object* object, int add_uid );
ObjectM* Object_remove( ObjectM* first, Object* object );

#endif //_ECE_SPRITS_OBJECTS_H_
