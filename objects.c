#include "inc/objects.h"

void Object_computeMomentOfInertia( Object* obj ) {
	obj->moment_of_inertia = obj->mass * ( obj->size.x * obj->size.x + obj->size.y * obj->size.y ) / 12;
}

void Object_init( Object *obj ) {
	obj->cp.x = 100;
	obj->cp.y = 0;
	obj->force.x = 0;
	obj->force.y = 0;
	obj->a.x = 0;
	obj->a.y = 0;
	obj->v.x = 0;
	obj->v.y = 0.11;
	obj->torque = 0;
	obj->moment_of_inertia = 0;
	obj->av = 0;
	obj->oldcp.x = 0;
	obj->oldcp.y = 0;
	obj->direction = 1;
	obj->angle = 0;
	obj->mass = 5;
	obj->restitution = 0.3;
	obj->counter = 0;
	obj->state = STATE_WALKING;
	obj->capacities.digging = 0;
	obj->capacities.blowing = 0;
	obj->capacities.building = 0;
	obj->capacities.angle = 0;
	obj->capacities.distance = 0;
	obj->capacities.left = 0;
	obj->capacities.direction = (Vector2) { 0, 0 };
	obj->capacities.start_points[ 0 ] = (Vector2) { 0, 0 };
	obj->capacities.start_points[ 1 ] = (Vector2) { 0, 0 };
	obj->capacities.delta_combined = (Vector2Char) { 0, 0 };
	obj->selected = 0;
	Object_computeMomentOfInertia( obj );
}

/*
 * Récupération d'un identifiant unique pour un objet
 */
int Object_getUID( ObjectM* first ) {
	// Déclaration des variables utiles
	int uid, is_unique;
	ObjectM* maillon;

	// Tant que l'UID n'est pas unique
	do {
		// On met is_unique à un
		is_unique = 1;

		// On génère un ID aléatoirement, entre 1 et RAND_MAX
		uid = rand();

		// On le compare aux ID des autres rendez-vous
		for( maillon = first; maillon != NULL; maillon = maillon->next ) {
			if( maillon->obj != NULL && maillon->obj->id == uid )
				is_unique = 0;
		}
	} while( is_unique == 0 );

	// On retourne l'UID
	return uid;
}

/*
 * Recherche du pointeur du maillon d'un objet d'ID donné
 */
ObjectM* Object_findMaillon( ObjectM* first, int id ) {
	// Déclaration des variables utiles
	ObjectM* maillon;

	// On parcours les objet
	for( maillon = first; maillon != NULL; maillon = maillon->next ) {
		// Si c'est le bon ID
		if( maillon->obj != NULL && maillon->obj->id == id )
			return maillon; // On retorune le pointeur
	}

	// Echec, on retourne NULL
	return first;
}

/*
 * Ajout d'un objet à une liste de maillons ObjectM
 */
ObjectM* Object_add( ObjectM* first, Object* object, int add_uid ) {
	// Déclaration des variables utiles
	ObjectM *new, *last;

	// Si on a un maillon incorrect
	if( first == NULL ) {
		// On l'alloue
		first = ObjectM_allocate();

		// S'il est toujours incorrect (erreur d'allocation)
		if( first == NULL ) {
			// Echec, on retourne NULL
			return NULL;
		}
	}

	// S'il le faut, on génère un UID pour le rendez-vous
	if( add_uid )
		object->id = Object_getUID( first );

	// Si le maillon est vide
	if( first->obj == NULL ) {
		// On l'utilise
		new = first;
	} else {
		// Sinon, on alloue un nouveau maillon
		new = ObjectM_allocate();
	}

	// Si l'allocation n'a pas échouée, ou que le 1er maillon est correct
	if( new != NULL ) {
		// On parcours les objet jusqu'à trouver le dernier
		last = first;

		if( last->next != NULL ) {
			do {
				last = last->next;
			} while( last->next != NULL );
		}

		if( first != new ) {
			last->next = new;
			new->prev = last;
		}

		new->obj = object;

	}

	return first;
}

/*
 * Suppression d'un objet dans une liste de maillons ObjectM
 */
ObjectM* Object_remove( ObjectM* first, Object* object ) {
	// Déclaration des variables utiles
	ObjectM *maillon, *last;

	// Si on a bien un maillon correct
	if( first != NULL ) {

		// On initialise last
		maillon = first;

		// On parcours les maillons de objet jusqu'à trouver celui qui correspond au objet que l'on souhaite supprimer
		do {
			last = maillon;
			maillon = maillon->next;
		} while( maillon && last->obj != NULL && last->obj->id != object->id );

		// Si on est bien tombé sur le objet à supprimer
		if( last->obj != NULL && last->obj->id == object->id ) {
			// On lie le maillon précédent au suivant et inversement, si possible
			if( last->prev != NULL ) {
				if( last->next != NULL )
					last->prev->next = last->next;
				else
					last->prev->next = NULL;
			}

			if( last->next != NULL ) {
				if( last->prev != NULL )
					last->next->prev = last->prev;
				else
					last->next->prev = NULL;
			}

			// Si le maillon du objet qu'on cherche à supprimer n'est pas le 1er
			if( last != first ) {
				// Alors on libère le maillon
				ObjectM_free( last, 1 );

				// Et on retourne le 1er maillon
				return first;
			} else {
				// Sinon, on libère le maillon
				ObjectM_free( last, 1 );

				// Et on retourne le maillon suivant
				return maillon;
			}
		}
	}

	// Sinon on retourne simplement le pointeur actuel
	return first;
}