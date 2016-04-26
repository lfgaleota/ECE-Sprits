#ifndef _ECE_SPRITS_MEMORY_H_
#define _ECE_SPRITS_MEMORY_H_

#include <stdlib.h>
#include "structures.h"

Object* Object_allocate();
void Object_free( Object* obj );

ObjectM* ObjectM_allocate();
void ObjectM_free( ObjectM* maillon, int free_object );
void ObjectM_freeAll( ObjectM* first, int free_object );


Material* Material_allocate();
void Material_free( Material* mat );

MaterialM* MaterialM_allocate();
void MaterialM_free( MaterialM* maillon, int free_mat );
void MaterialM_freeAll( MaterialM* first, int free_mat );
#endif //_ECE_SPRITS_MEMORY_H_
