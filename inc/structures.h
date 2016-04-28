#ifndef _ECE_SPRITS_STRUCTURES_H_
#define _ECE_SPRITS_STRUCTURES_H_

#include <allegro.h>

#ifdef __WIN32
#define SLEEP( ms ) rest( ms )
#else
#define SLEEP( ms ) usleep( ms * 1000 )
#endif

#define FLOAT_AROUND( f1, f2, precision ) ( f1 >= f2 - precision && f1 <= f2 + precision )

#define BUFFER_SIZE 255

#define P_UP_LEFT 0
#define P_UP_RIGHT 1
#define P_DOWN_RIGHT 2
#define P_DOWN_LEFT 3

#define UP 0
#define UP_LEFT 1
#define LEFT 2
#define DOWN_LEFT 3
#define DOWN 4
#define DOWN_RIGHT 5
#define RIGHT 6
#define UP_RIGHT 7

#define SIDE_UP 0
#define SIDE_DOWN 1
#define SIDE_LEFT 2
#define SIDE_RIGHT 3

#define STATUS_ALREADY_BLOCKED 0x1
#define STATUS_WALL 0x2
#define STATUS_IS_SIDE_OF_WALKING 0x4
#define STATUS_IS_SIDE_OF_FLYING 0x8

#define MAX_STEP 7

#define STATE_WALKING 0
#define STATE_FALLING 1
#define STATE_DEAD 2
#define STATE_EXITING 3
#define STATE_DYING 4
#define STATE_FALLDYING 5
#define STATE_BUILDING 6
#define STATE_DIGGING 7
#define STATE_BLOWING 8

#define COLOR_NEUTRAL makecol( 0, 0, 0 )
#define COLOR_WALL makecol( 255, 255, 255 )
#define COLOR_BREAKABLEWALL makecol( 0, 0, 255 )
#define COLOR_DEATHZONE makecol( 255, 0, 0 )
#define COLOR_EXIT makecol( 0, 255, 255 )

#define COLOR_FOREGROUND_NEUTRAL makecol( 255, 0, 255 )
#define COLOR_FOREGROUND_BUILD makecol( 128, 128, 128 )

#define DIRECTION_LEFT 0
#define DIRECTION_RIGHT 1

#define X_SIGNIFICANT 4
#define Y_SIGNIFICANT 4

#define Y_FALLDAMAGE 10

#define QUIT_WIN 1
#define QUIT_FAIL 2
#define QUIT_DELIBERATE 3

#define MAX_DIGGING 80
#define MAX_BUILDING 45

#define DIGGING_DISTANCE 10
#define BUILDING_DISTANCE 2
#define BLOWING_DISTANCE 150
#define BLOWING_WIDTH 10

#define WIND_FORCE_X 12
#define WIND_FORCE_Y 12

#define CAPACITY_BUILD 0
#define CAPACITY_DIG 1
#define CAPACITY_BLOW 2

typedef struct MenuItem MenuItem;
typedef struct CircularMenu CircularMenu;

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

typedef struct Capacities Capacities;
struct Capacities {
	unsigned char digging;
	unsigned char building;
	unsigned char blowing;
	float angle;
	int left;
	Position start_points[ 2 ];
	Vector2 direction;
	float distance;
	Vector2Char delta_combined;
};

typedef struct CapacitiesNumber CapacitiesNumber;
struct CapacitiesNumber {
	unsigned char digging;
	unsigned char building;
	unsigned char blowing;
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
	float diagonal;
	Direction delta;
	Direction dir;
	char direction;
	char should_move;
	float torque;
	float moment_of_inertia;
	float av;
	float angle;
	float restitution;
	unsigned short mass;
	char state;
	char selected;
	Capacities capacities;
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
	BITMAP* wind_col;
	BITMAP* back;
	BITMAP* fore;
	BITMAP* wall;
	BITMAP* deathzone;
	BITMAP** arrow;
	BITMAP* capacity_build;
	BITMAP* capacity_dig;
	BITMAP* capacity_blow;
	Frames start;
	Frames exit;
	Frames stickmen_walking;
	Frames stickmen_falling;
	Frames stickmen_dying;
	Frames stickmen_falldying;
};

typedef struct Inputs Inputs;
struct Inputs {
	int prev_mouse_l;
	int mouse_l;
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
	Position start;
	Position exit;
	char quit;
	char win;
	unsigned short nb_stickmen_max;
	unsigned short nb_stickmen_added;
	unsigned short nb_stickmen_arrived;
	unsigned short nb_stickmen_should_arrive;
	unsigned short nb_stickmen_dead;
	unsigned short counter_stickmen_arrival;
	unsigned short counter_stickmen_arrival_max;
	CapacitiesNumber capacities;
	CircularMenu* capacities_menu;
	CircularMenu* directions_menu;
	Inputs inputs;
};

struct MenuItem {
	BITMAP *icon;
	char *tooltip;
	int bg_color;
	int bg_sel_color;
	int text_color;

	char ( *callback )( Level* level, Object* obj, int index, int x, int y );

	int center_x;
	int center_y;
	int icon_x;
	int icon_y;
};

struct CircularMenu {
	char opened;
	int count;
	int inner_radius;
	int medium_radius;
	int outer_radius;
	float angle;
	int screen_x;
	int screen_y;
	BITMAP *bmp;
	MenuItem *items;
};

#endif //_ECE_SPRITS_STRUCTURES_H_
