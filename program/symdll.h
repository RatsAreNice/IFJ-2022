// Double linked list pre tabulky symbolov.
// Hlavičkový súbor upravený z prvej IAL úlohy
// Autor: Richard Blažo (xblazo00)
#ifndef SYMDLL_H
#define SYMDLL_H
#include"symtable.h"
#include<stdio.h>
#include<stdlib.h>

typedef struct symDLLElement {
    bst_node_t* symtable;
	funData_t* fundata;
	struct symDLLElement *previousElement;
	struct symDLLElement *nextElement;
} *symDLLElementPtr;

typedef struct symDLList{
	symDLLElementPtr firstElement;

	symDLLElementPtr activeElement;

	symDLLElementPtr lastElement;
} symDLList_t;

void symDLL_Init( symDLList_t * );

void symDLL_Dispose( symDLList_t * );

void symDLL_InsertLast( symDLList_t *, bst_node_t* , funData_t* );

void symDLL_First( symDLList_t * );

void symDLL_Last( symDLList_t * );

bst_node_t * symDLL_GetFirst( symDLList_t *list);

void symDLL_DeleteLast( symDLList_t * );

void symDLL_GetValue( symDLList_t *, bst_node_t ** );

void symDLL_Next( symDLList_t * );

void symDLL_Previous( symDLList_t * );

int symDLL_IsActive( symDLList_t * );
#endif