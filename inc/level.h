#ifndef _ECE_SPRITS_LEVEL_H_
#define _ECE_SPRITS_LEVEL_H_

#include <stdio.h>
#include <allegro.h>
#include <loadpng.h>
#include <jpgalleg.h>
#include "structures.h"

BITMAP** Level_loadBitmaps( char* path, unsigned short count );
Frames Level_loadFrames( char* path, unsigned short count );

#endif //_ECE_SPRITS_LEVEL_H_
