#ifndef _ECE_SPRITS_SAVE_H_
#define _ECE_SPRITS_SAVE_H_

#include <stdio.h>
#include "structures.h"

char Save_save( Save* save );
Save Save_load( unsigned char id );

#endif
