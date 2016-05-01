#ifndef _ECE_SPRITS_SAVE_H_
#define _ECE_SPRITS_SAVE_H_

#include <stdio.h>

typedef struct Save Save;
struct Save {
	unsigned char id;
	unsigned char standard_level;
};

char Save_save( Save* save );
Save Save_load( unsigned char id );

#endif
