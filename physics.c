#include "inc/physics.h"

float Vector_norm( Vector2 vec ) {
	return sqrtf( vec.x * vec.x + vec.y * vec.y );
}

void Physic_computeDelta( Object* obj ) {
	obj->delta.x = floorf( obj->cp.x ) - floorf( obj->oldcp.x );
	obj->delta.y = floorf( obj->cp.y ) - floorf( obj->oldcp.y );

	if( obj->delta.x > 0 )
		obj->dir.x = 1;
	else if( obj->delta.x < 0 )
		obj->dir.x = -1;
	else
		obj->dir.x = 0;
	if( obj->delta.y > 0 )
		obj->dir.y = 1;
	else if( obj->delta.y < 0 )
		obj->dir.y = -1;
	else
		obj->dir.y = 0;
}

void Physic_updatePosition( Object* obj ) {
	/*  p2 p3
	 *  +---+
	 *  |   |  cp
	 *  | x |_ angle
	 *  |   | |
	 *  +---+----
	 *  p1 p4
	 */
	float diagonal = Vector_norm( obj->size );
	float diagonal_2 = diagonal / 2;
	float inner_angle = atanf( obj->size.x / obj->size.y );

	Vector2 CPtoP1 = { - diagonal_2 * cosf( obj->angle - inner_angle ), diagonal_2 * sinf( obj->angle - inner_angle ) };
	Vector2 CPtoP2 = { diagonal_2 * cosf( obj->angle + inner_angle ), - diagonal_2 * sinf( obj->angle + inner_angle ) };

	obj->p[ P_UP_LEFT ].x = obj->cp.x + CPtoP1.x;
	obj->p[ P_UP_LEFT ].y = obj->cp.y + CPtoP1.y;

	obj->p[ P_UP_RIGHT ].x = obj->cp.x + CPtoP2.x;
	obj->p[ P_UP_RIGHT ].y = obj->cp.y + CPtoP2.y;

	obj->p[ P_DOWN_RIGHT ].x = obj->cp.x - CPtoP1.x;
	obj->p[ P_DOWN_RIGHT ].y = obj->cp.y - CPtoP1.y;

	obj->p[ P_DOWN_LEFT ].x = obj->cp.x - CPtoP2.x;
	obj->p[ P_DOWN_LEFT ].y = obj->cp.y - CPtoP2.y;

	obj->propcp = (Vector2Char) { 0, 0 };
}

void Physic_initMovement( Object *obj, Vector2 gravity, Vector2 movement ) {
	Physic_updatePosition( obj );

	obj->force.x = gravity.x;
	obj->force.y = gravity.y;

	if( obj->v.y >= -0.1 && obj->v.y <= 0.1 ) { //FLOAT_AROUND( obj->v.y, 0.0, 0.1 )
		obj->v.x = ( obj->direction ? 1 : -1 ) * movement.x;
		obj->v.y = ( obj->direction ? 1 : -1 ) * movement.y;
	}
}

void Physic_compute( Object *obj, double dt ) {
	//obj->torque = obj->cp.x * obj->force.y - obj->cp.y * obj->force.x;
	obj->a.x = obj->force.x / obj->mass;
	obj->a.y = obj->force.y / obj->mass;
	obj->v.x += obj->a.x * dt;
	obj->v.y += obj->a.y * dt;
	obj->oldcp.x = obj->cp.x;
	obj->oldcp.y = obj->cp.y;
	obj->cp.x += obj->v.x * dt;
	obj->cp.y += obj->v.y * dt;
	/*float aa = obj->torque / obj->moment_of_inertia;
	obj->av += aa * dt;
	obj->angle += obj->av * dt;*/

	Physic_computeDelta( obj );
}