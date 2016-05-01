#include "inc/memory.h"

/*
 * Allocation d'un objet
 */
Object* Object_allocate() {
	return ( (Object*) calloc( 1, sizeof( Object ) ) );
}

/*
 * Libération d'un objet
 */
void Object_free( Object* obj ) {
	// On libère l'objet
	free( obj );
}

/*
 * Allocation d'un maillon objet
 */
ObjectM* ObjectM_allocate() {
	ObjectM* maillon = calloc( 1, sizeof( ObjectM ) );

	if( maillon != NULL )  {
		maillon->prev = NULL;
		maillon->obj = NULL;
		maillon->next = NULL;

		return maillon;
	}

	return NULL;
}

/*
 * Libération d'un maillon objet
 */
void ObjectM_free( ObjectM* maillon, int free_object ) {
	// Si le maillon est correct
	if( maillon != NULL ) {
		// S'il le faut, on libère l'objet du maillon
		if( free_object )
			Object_free( maillon->obj );

		// On libère l'espace du maillon
		free( maillon );
	}
}

/*
 * Libération de tous les maillons objet
 */
void ObjectM_freeAll( ObjectM* first, int free_object ) {
	// Déclaration des variables utiles
	ObjectM *maillon, *next;

	// On parcours les maillons
	for( maillon = first; maillon != NULL; maillon = next ) {
		// On récupère le pointeur vers le prochain maillon
		next = maillon->next;

		// On libère le maillon
		ObjectM_free( maillon, free_object );
	}
}