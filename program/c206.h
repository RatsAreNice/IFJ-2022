#ifndef c206_H
#define c206_H
#include "ASS.h"
#include "scanner.h"

typedef struct unit{
    int ttyp;
    ASSnode_t* uzol;
    token_t vvalue;
} unit_t;

typedef struct DLLElement {
	unit_t data;
	struct DLLElement *previousElement;
	struct DLLElement *nextElement;
} *DLLElementPtr;

typedef struct {
	
	DLLElementPtr firstElement;
	
	DLLElementPtr activeElement;
	
	DLLElementPtr lastElement;
} DLList;




void DLL_Error();
void DLL_Init( DLList *list );
void DLL_Dispose( DLList *list );
void DLL_InsertFirst( DLList *list, unit_t data );
void DLL_InsertLast( DLList *list, unit_t data );
void DLL_First( DLList *list );
void DLL_Last( DLList *list );
void DLL_GetFirst( DLList *list, unit_t *dataPtr );
void DLL_GetLast( DLList *list, unit_t *dataPtr );
void DLL_DeleteFirst( DLList *list );
void DLL_DeleteLast( DLList *list );
void DLL_DeleteAfter( DLList *list );
void DLL_DeleteBefore( DLList *list );
void DLL_InsertAfter( DLList *list, unit_t data );
void DLL_InsertBefore( DLList *list, unit_t data );
void DLL_GetValue( DLList *list, unit_t *dataPtr );
void DLL_SetValue( DLList *list, unit_t data );
void DLL_Next( DLList *list );
void DLL_Previous( DLList *list );
int DLL_IsActive( DLList *list );
#endif