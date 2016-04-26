#include "inc/memory.h"

/*
 * Allocation d'un objet
 */
Object* Object_allocate() {
	return ( (Object*) malloc( sizeof( Object ) ) );
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

/*
 * Allocation d'un matériau
 */
Material* Material_allocate() {
	return ( (Material*) malloc( sizeof( Material ) ) );
}

/*
 * Libération d'un matériau
 */
void Material_free( Material* mat ) {
	// On libère le matériau
	free( mat );
}

/*
 * Allocation d'un maillon de matériau
 */
MaterialM* MaterialM_allocate() {
	MaterialM* MaterialM = malloc( sizeof( MaterialM ) );

	if( MaterialM != NULL )  {
		MaterialM->prev = NULL;
		MaterialM->mat = NULL;
		MaterialM->next = NULL;

		return MaterialM;
	}

	return NULL;
}

/*
 * Libération d'un maillon matériau
 */
void MaterialM_free( MaterialM* maillon, int free_matect ) {
	// Si le maillon est correct
	if( maillon != NULL ) {
		// S'il le faut, on libère le matériau du maillon
		if( free_matect )
			Material_free( maillon->mat );

		// On libère l'espace du maillon
		free( maillon );
	}
}

/*
 * Libération de tous les maillons de matériaux
 */
void MaterialM_freeAll( MaterialM* first, int free_matect ) {
	// Déclaration des variables utiles
	MaterialM *maillon, *next;

	// On parcours les maillons
	for( maillon = first; maillon != NULL; maillon = next ) {
		// On récupère le pointeur vers le prochain maillon
		next = maillon->next;

		// On libère le maillon
		MaterialM_free( maillon, free_matect );
	}
}