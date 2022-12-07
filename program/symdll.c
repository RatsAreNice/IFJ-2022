// Nazov projektu: IFJ22
// Autor: Richard Blažo (xblazo00)
// Double linked list pre tabulky symbolov.
// Implementácia z prvej IAL úlohy.
// Autor: Richard Blažo (xblazo00)
#include "symdll.h"

void symDLL_Init( symDLList_t *list ) {
	list->firstElement = NULL;
	list->activeElement = NULL;
	list->lastElement = NULL;
}

void symDLL_Dispose( symDLList_t *list ) {
	if(list->firstElement != NULL)
	{
		list->activeElement = list->firstElement;
		while (list->activeElement != NULL)
		{
			symDLLElementPtr temp = list->activeElement;
			list->activeElement = list->activeElement->nextElement;
			free(temp);
		}
		
	}
	list->firstElement = NULL;
	list->activeElement = NULL;
	list->lastElement = NULL;
}

void symDLL_InsertLast( symDLList_t *list, bst_node_t* symtable, funData_t* fundata ) {
	symDLLElementPtr new = malloc(sizeof(struct symDLLElement));
	if(new == NULL)
	{
		fprintf(stderr, "Malloc failed.\n");
        exit(99);
	}
	new->fundata = fundata;
	new->symtable = symtable;
	new->nextElement = NULL;
	new->previousElement = list->lastElement;
	// Ak je DLL prázdny -> novy element == firstElement == lastElement
	if(list->lastElement == NULL)
	{
		list->firstElement = new;
	}
	else
		list->lastElement->nextElement = new;
	list->lastElement = new;
}

void symDLL_First( symDLList_t *list ) {
	list->activeElement = list->firstElement;
}

void symDLL_Last( symDLList_t *list ) {
	list->activeElement = list->lastElement;
}

void symDLL_DeleteLast( symDLList_t *list ) {
	symDLLElementPtr temp = list->lastElement;
	if(list->lastElement == NULL)
		return;
	if(list->lastElement == list->activeElement)
		list->activeElement = NULL;
	// Ak jediny element v DLL -> lastElement == firstElement -> NULL
	if(list->firstElement == list->lastElement)
	{
		list->lastElement = NULL;
		list->firstElement = NULL;
	}
	else
	{
		list->lastElement = list->lastElement->previousElement;
		list->lastElement->nextElement = NULL;
	}
	bst_dispose(&(temp->symtable));
	free(temp);
}

bst_node_t * symDLL_GetFirst( symDLList_t *list) {
	if (list->firstElement == NULL)
	{
		fprintf(stderr, "DLL empty.\n");
        exit(99);
	}
	return list->firstElement->symtable;
}

void symDLL_GetValue( symDLList_t *list, bst_node_t **dataPtr ) {
	if(symDLL_IsActive(list))
		*dataPtr = list->activeElement->symtable;
	else
    {
        fprintf(stderr, "DLL not active.\n");
        exit(99);
    }
}

void symDLL_Previous( symDLList_t *list ) {
	if (symDLL_IsActive(list))
		list->activeElement = list->activeElement->previousElement;
}

void symDLL_Next( symDLList_t *list ) {
	if (symDLL_IsActive(list))
		list->activeElement = list->activeElement->nextElement;
}

int symDLL_IsActive( symDLList_t *list ) {
	return list->activeElement == NULL ? 0 : 1;
}