#include<stdio.h>
#include<stdlib.h>
#include"c206.h"

#define FALSE 0
#define TRUE 1


extern int error_flag;



/** Prvek dvousměrně vázaného seznamu. */


/** Dvousměrně vázaný seznam. */


int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
	DLLElementPtr lolec;
	while(list->firstElement != NULL){
		lolec = list->firstElement->nextElement;
		free(list->firstElement);
		list->firstElement = lolec;
	}
	list->lastElement=NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
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

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
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

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, unit_t *dataPtr ) {
	if(list->firstElement == NULL){
		DLL_Error();
	}
	else{
		*dataPtr = list->firstElement->data;
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, unit_t *dataPtr ) {
	if(list->firstElement == NULL){
		DLL_Error();
	}
	else{
		*dataPtr = list->lastElement->data;
	}
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
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

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
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

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
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

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
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

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
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

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
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

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, unit_t *dataPtr ) {
	if(list->activeElement == NULL){
		DLL_Error();
	}
	else{
		*dataPtr = list->activeElement->data;
	}
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, unit_t data ) {
	if(list->activeElement != NULL){
		list->activeElement->data = data;
	}
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if(list->activeElement != NULL){
		list->activeElement = list->activeElement->nextElement;
	}
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	if(list->activeElement != NULL){
		list->activeElement = list->activeElement->previousElement;
	}
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	return list->activeElement != NULL;
}

/* Konec c206.c */
