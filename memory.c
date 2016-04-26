#include "inc/memory.h"

/*
 * Allocation de Object
 */
Object* Object_allocate() {
	return ( (Object*) malloc( sizeof( Object ) ) );
}

/*
 * Libération de Object
 */
void Object_free( Object* obj ) {
	// On libère l'objet
	free( obj );
}

/*
 * Allocation d'un maillon Object
 */
ObjectM* ObjectM_allocate() {
	ObjectM* ObjectM = malloc( sizeof( ObjectM ) );

	if( ObjectM != NULL )  {
		ObjectM->prev = NULL;
		ObjectM->obj = NULL;
		ObjectM->next = NULL;

		return ObjectM;
	}

	return NULL;
}

/*
 * Libération d'un maillon Object
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
 * Libération de tous les maillons Object
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