#ifndef _ECE_SPRITS_STRUCTURES_H_
#define _ECE_SPRITS_STRUCTURES_H_

#include <allegro.h>

#ifdef __WIN32
#define SLEEP( ms ) rest( ms )
#else
#define SLEEP( ms ) usleep( ms * 1000 )
#endif

#define FLOAT_AROUND( f1, f2, precision ) ( f1 >= f2 - precision && f1 <= f2 + precision )

#define P_UP_LEFT 0
#define P_UP_RIGHT 1
#define P_DOWN_RIGHT 2
#define P_DOWN_LEFT 3

#define SIDE_UP 0
#define SIDE_DOWN 1
#define SIDE_LEFT 2
#define SIDE_RIGHT 3

#define STATUS_ALREADY_BLOCKED 0x1
#define STATUS_WALL 0x2
#define STATUS_IS_SIDE_OF_WALKING 0x4

#define MAX_STEP 7

#define STATE_WALKING 0
#define STATE_FALLING 1
#define STATE_DEAD 2

#define COLOR_WALL makecol( 255, 255, 255 )

#define X_SIGNIFICANT 2
#define Y_SIGNIFICANT 2

typedef struct Vector2 Vector2;
typedef struct Vector2 Force;
typedef struct Vector2 Acceleration;
typedef struct Vector2 Velocity;
typedef struct Vector2 Position;
typedef struct Vector2 Size;
struct Vector2 {
	float x;
	float y;
};

typedef struct Vector2Char Vector2Char;
typedef struct Vector2Char Direction;
struct Vector2Char {
	char x;
	char y;
};

typedef struct Object Object;
struct Object {
	unsigned int id;
	Position cp;
	Position oldcp;
	Position p[ 4 ];
	Vector2Char propcp;
	Force force;
	Acceleration a;
	Velocity v;
	Size size;
	Direction delta;
	Direction dir;
	char direction;
	float torque;
	float moment_of_inertia;
	float av;
	float angle;
	float restitution;
	unsigned short mass;
	char state;
	BITMAP* bmp;
	unsigned short counter;
};

typedef struct ObjectM ObjectM;
struct ObjectM {
	ObjectM* prev;
	Object* obj;
	ObjectM* next;
};

typedef struct Frames Frames;
struct Frames {
	BITMAP** bmps;
	unsigned short count;
};

typedef struct Bitmaps Bitmaps;
struct Bitmaps {
	BITMAP* page;
	BITMAP* col;
	BITMAP* stick_col;
	BITMAP* back;
	BITMAP* fore;
	Frames stickmen_walking;
	Frames stickmen_falling;
};

typedef struct Level Level;
struct Level {
	Bitmaps bmps;
	ObjectM* stickmen;
	float dt;
	float slow_dt;
	float fast_dt;
	Vector2 gravity;
	Vector2 movement;
	unsigned short nb_stickmen_max;
	unsigned short nb_stickmen_added;
	unsigned short nb_stickmen_arrived;
	unsigned short nb_stickmen_dead;
	unsigned short counter_stickmen_arrival;
	unsigned short counter_stickmen_arrival_max;
};

#endif //_ECE_SPRITS_STRUCTURES_H_
