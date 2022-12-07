//Autor: Oliver Nemcek <xnemce08>
//Sablona prevzata z domacej ulohy predmetu IAL

#include<stdio.h>
#include<stdlib.h>
#include"c206.h"

#define FALSE 0
#define TRUE 1


extern int error_flag;
int error_flag;
int solved;

void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

void DLL_Init( DLList *list ) {
	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
}

void DLL_Dispose( DLList *list ) {
	DLLElementPtr lolec;
	while(list->firstElement != NULL){
		lolec = list->firstElement->nextElement;
		free(list->firstElement);
		list->firstElement = lolec;
	}
	list->lastElement=NULL;
}

void DLL_InsertFirst( DLList *list, unit_t data ) {
	DLLElementPtr newElemPtr;
	newElemPtr = malloc(sizeof(struct DLLElement));	//vytvorime novy prvok
	if(newElemPtr == NULL){
		DLL_Error();	//osetrenie chyby mallocu
	}
	else{
		newElemPtr->data = data;	//do noveho prvku vlozime prislusne data
		newElemPtr->nextElement = list->firstElement;
		newElemPtr->previousElement = NULL;
		if(list->firstElement != NULL){
			list->firstElement->previousElement = newElemPtr;
		}
		else{
			list->lastElement = newElemPtr;
		}
		list->firstElement = newElemPtr;
	}
}

void DLL_InsertLast( DLList *list, unit_t data ) {
	DLLElementPtr newElemPtr;
	newElemPtr = malloc(sizeof(struct DLLElement));	//vytvorime novy prvok
	if(newElemPtr == NULL){
		DLL_Error();	//osetrenie chyby mallocu
	}
	else{
		newElemPtr->data = data; //do noveho prvku vlozime prislusne data
		newElemPtr->previousElement = list->lastElement;
		newElemPtr->nextElement = NULL;
		if(list->lastElement != 0){
			list->lastElement->nextElement = newElemPtr;
		}
		else{
			list->firstElement = newElemPtr;
		}
		list->lastElement = newElemPtr;
	}
}

void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;
}

void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;
}

void DLL_GetFirst( DLList *list, unit_t *dataPtr ) {
	if(list->firstElement == NULL){
		DLL_Error();
	}
	else{
		*dataPtr = list->firstElement->data;
	}
}

void DLL_GetLast( DLList *list, unit_t *dataPtr ) {
	if(list->firstElement == NULL){
		DLL_Error();
	}
	else{
		*dataPtr = list->lastElement->data;
	}
}

void DLL_DeleteFirst( DLList *list ) {
	DLLElementPtr elemPtr;
	if(list->firstElement != NULL){
		elemPtr = list->firstElement;
		if(list->activeElement == list->firstElement){
			list->activeElement = NULL;
		}
		if(list->firstElement == list->lastElement){
			list->firstElement = NULL;
			list->lastElement = NULL;
		}
		else{
			list->firstElement = list->firstElement->nextElement;
			list->firstElement->previousElement = NULL;
		}
		free(elemPtr);

	}
}

void DLL_DeleteLast( DLList *list ) {
	DLLElementPtr elemPtr;
	if(list->lastElement != NULL){
		elemPtr = list->lastElement;
		if(list->activeElement == list->lastElement){
			list->activeElement = NULL;
		}
		if(list->lastElement == list->firstElement){
			list->firstElement = NULL;
			list->lastElement = NULL;
		}
		else{
			list->lastElement = list->lastElement->previousElement;
			list->lastElement->nextElement = NULL;
		}
		free(elemPtr);
	}
}

void DLL_DeleteAfter( DLList *list ) {
	if(list->activeElement != NULL){
		if(list->activeElement->nextElement != NULL){
			DLLElementPtr elemPtr;
			elemPtr = list->activeElement->nextElement;
			list->activeElement->nextElement = elemPtr->nextElement;
			if(elemPtr == list->lastElement){
				list->lastElement = list->activeElement; //ak je ruseny posledny prvok zoznamu, novy posledny bude jeho predosly (teda ten co je prave aktivny)
			}
			else{
				elemPtr->nextElement->previousElement = list->activeElement;
			}
			free(elemPtr);
		}
	}
}

void DLL_DeleteBefore( DLList *list ) {
	if(list->activeElement != NULL){
		if(list->activeElement->previousElement != NULL){
			DLLElementPtr elemPtr;
			elemPtr = list->activeElement->previousElement;
			list->activeElement->previousElement = elemPtr->previousElement;
			if(elemPtr == list->firstElement){
				list->firstElement = list->activeElement;
			}
			else{
				elemPtr->previousElement->nextElement = list->activeElement;
			}
			free(elemPtr);
		}
	}
}

void DLL_InsertAfter( DLList *list, unit_t data ) {
	if(list->activeElement !=NULL){
		DLLElementPtr newElement;
		newElement = malloc(sizeof(struct DLLElement));
		if(newElement == NULL){
			DLL_Error();
		}
		else{
			newElement->data = data;
			newElement->nextElement = list->activeElement->nextElement;
			newElement->previousElement = list->activeElement;
			list->activeElement->nextElement = newElement;
			if(list->activeElement == list ->lastElement){
				list->lastElement = newElement;
			}
			else{
				newElement->nextElement->previousElement = newElement;
			}
		}
	}
}

void DLL_InsertBefore( DLList *list, unit_t data ) {
	if(list->activeElement != NULL){
		DLLElementPtr newElement;
		newElement = malloc(sizeof(struct DLLElement));
		if(newElement == NULL){
			DLL_Error();
		}
		else{
			newElement->data = data;
			newElement->nextElement = list->activeElement;
			newElement->previousElement = list->activeElement->previousElement;
			list->activeElement->previousElement = newElement;
			if(list->firstElement == list->activeElement){
				list->firstElement = newElement;
			}
			else{
				newElement->previousElement->nextElement = newElement;
			}
		}
	}
}

void DLL_GetValue( DLList *list, unit_t *dataPtr ) {
	if(list->activeElement == NULL){
		DLL_Error();
	}
	else{
		*dataPtr = list->activeElement->data;
	}
}

void DLL_SetValue( DLList *list, unit_t data ) {
	if(list->activeElement != NULL){
		list->activeElement->data = data;
	}
}


void DLL_Next( DLList *list ) {
	if(list->activeElement != NULL){
		list->activeElement = list->activeElement->nextElement;
	}
}

void DLL_Previous( DLList *list ) {
	if(list->activeElement != NULL){
		list->activeElement = list->activeElement->previousElement;
	}
}

int DLL_IsActive( DLList *list ) {
	return list->activeElement != NULL;
}