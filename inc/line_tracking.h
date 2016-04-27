#ifndef _ECE_SPRITS_LINE_TRACKING_H_
#define _ECE_SPRITS_LINE_TRACKING_H_

#include <allegro.h>
#include "structures.h"

void TrackLine( Level* level, Object* obj, int x1, int y1, int x2, int y2, int (*callback) ( Level*, Object*, int, int ) );

#endif //_ECE_SPRITS_LINE_TRACKING_H_
