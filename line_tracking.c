#include "inc/line_tracking.h"

/*void TrackLine( Object* obj, BITMAP* col, int x1, int y1, int x2, int y2, int dx_dir, int dy_dir, int (*callback) ( Object*, BITMAP*, int, int, int, int ) ) {
	float i, j, m;

	if( x1 == x2 && y1 == y2 ) {
		return;
	} else if( x1 == x2 ) {
		for( i = y1; dy_dir * i <= dy_dir * y2; i += dy_dir ) {
			if( callback( obj, col, dx_dir, dy_dir, x1, floorf( i ) ) )
				return;
		}
	} else if( y1 == y2 ) {
		for( j = x1; dx_dir * j <= dx_dir * x2; j += dx_dir ) {
			if( callback( obj, col, dx_dir, dy_dir, floorf( j ), y1 ) )
				return;
		}
	} else {
		m = ( y2 - y1 ) / ( x2 - x1 );

		if( m > 1 ) {
			for( i = y1; dy_dir * i <= dy_dir * y2; i += dy_dir ) {
				j = i / m + x1;

				printf( "%f,%f", j, i );

				if( dx_dir * j <= dx_dir * x2 ) {
					if( callback( obj, col, dx_dir, dy_dir, floorf( j ), floorf( i ) ) )
						return;
				}
			}
		} else {
			for( j = x1; dx_dir * j <= dx_dir * x2; j += dx_dir ) {
				i = m * j;

				if( dy_dir * i <= dy_dir * y2 ) {
					if( callback( obj, col, dx_dir, dy_dir, floorf( j ), floorf( i ) ) )
						return;
				}
			}
		}
	}
}*/

void TrackLine( Level* level, Object* obj, int x1, int y1, int x2, int y2, int (*callback) ( Level*, Object*, int, int ) ) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	int i1, i2;
	int x, y;
	int dd;

	int code0, code1;
	int outcode;
	int xmax, xmin, ymax, ymin;
	int done = 0, accept = 0;

#define LTOP     0x8
#define LBOTTOM  0x4
#define LLEFT    0x2
#define LRIGHT   0x1

#define COMPCLIP(code, x, y)  \
      {                             \
	 code = 0;                  \
	 if (y < ymin)              \
	    code |= LTOP;            \
	 else if (y > ymax)         \
	    code |= LBOTTOM;         \
	 if (x < xmin)              \
	    code |= LLEFT;           \
	 else if (x > xmax)         \
	    code |= LRIGHT;          \
      }

	xmin = 0;
	xmax = SCREEN_W - 1;
	ymin = 0;
	ymax = SCREEN_W - 1;

	COMPCLIP(code0, x1, y1);
	COMPCLIP(code1, x2, y2);

	do {

		if (!(code0 | code1)) {
			// Trivially accept.
			accept = done = 1;
		}
		else if (code0 & code1) {
			// Trivially reject.
			done = 1;
		}
		else {
			// Didn't reject or accept, so do some calculations.
			outcode = code0 ? code0 : code1;  // pick one endpoint

			if (outcode & LTOP) {
				if (y2 == y1)
					x = x1;
				else
					x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
				y = ymin;
			}
			else if (outcode & LBOTTOM) {
				if (y2 == y1)
					x = x1;
				else
					x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
				y = ymax;
			}
			else if (outcode & LLEFT) {
				if (x2 == x1)
					y = y1;
				else
					y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
				x = xmin;
			}
			else {  // outcode & RIGHT
				if (x2 == x1)
					y = y1;
				else
					y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
				x = xmax;
			}

			if (outcode == code0) {
				x1 = x;
				y1 = y;
				COMPCLIP(code0, x1, y1);
			}
			else {
				x2 = x;
				y2 = y;
				COMPCLIP(code1, x2, y2);
			}
		}
	} while (!done);

#undef COMPCLIP
#undef LTOP
#undef LBOTTOM
#undef LLEFT
#undef LRIGHT

	if (!accept)
		return;

	// worker macro
#define DO_LINE( pri_sign, pri_c, pri_cond, sec_sign, sec_c, sec_cond )         \
		{                                                                       \
																				\
			i1 = 2 * d##sec_c;                                                  \
			dd = i1 - ( sec_sign ( pri_sign d##pri_c ) );                       \
			i2 = dd - ( sec_sign ( pri_sign d##pri_c ) );                       \
																				\
			x = x1;                                                             \
			y = y1;                                                             \
			while( pri_c pri_cond pri_c##2 ) {                                  \
				if( callback( level, obj, x, y ) )                              \
					return;                                                     \
																				\
				if( dd sec_cond 0 ) {                                           \
					sec_c = sec_c sec_sign 1;                                   \
					dd += i2;                                                   \
				} else                                                          \
					dd += i1;                                                   \
																				\
				pri_c = pri_c pri_sign 1;                                       \
			}                                                                   \
		}

	if( dx >= 0 ) {
		if( dy >= 0 ) {
			if( dx >= dy ) {
				// (x1 <= x2) && (y1 <= y2) && (dx >= dy)
				DO_LINE( +, x, <=, +, y, >= );
			} else {
				// (x1 <= x2) && (y1 <= y2) && (dx < dy)
				DO_LINE( +, y, <=, +, x, >= );
			}
		} else {
			if( dx >= -dy ) {
				// (x1 <= x2) && (y1 > y2) && (dx >= dy)
				DO_LINE( +, x, <=, -, y, <= );
			} else {
				// (x1 <= x2) && (y1 > y2) && (dx < dy)
				DO_LINE( -, y, >=, +, x, >= );
			}
		}
	} else {
		if( dy >= 0 ) {
			if( -dx >= dy ) {
				// (x1 > x2) && (y1 <= y2) && (dx >= dy)
				DO_LINE( -, x, >=, +, y, >= );
			} else {
				// (x1 > x2) && (y1 <= y2) && (dx < dy)
				DO_LINE( +, y, <=, -, x, <= );
			}
		} else {
			if( -dx >= -dy ) {
				// (x1 > x2) && (y1 > y2) && (dx >= dy)
				DO_LINE( -, x, >=, -, y, <= );
			} else {
				// (x1 > x2) && (y1 > y2) && (dx < dy)
				DO_LINE( -, y, >=, -, x, <= );
			}
		}
	}

#undef DO_LINE
}