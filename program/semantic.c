// Nazov projektu: IFJ22
// Autor: Richard Blažo (xblazo00)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symtable.h"
#include "scanner.h"

/// @brief Funkcia ktora urci ci je funkcia alebo premenna definovana
/// @param var Token s hodnotou mena funkcie.
/// @param symtable Tabulka symbolov v ktorej je hladane.
/// @return True ak existuje, false ak nie.
bool isDefined(token_t* var, bst_node_t* symtable)
{
    bst_node_t* result = bst_search(symtable, var->value);
    if(result == NULL)
        return false;
    else
        return true;
}

/// @brief Funkcia ktorá vráti ukazatel na funkciu v tabulke symbolov.
/// @param var Token s hodnotou mena funkcie.
/// @param symtable Tabulka symbolov v ktorej je hladane.
/// @return Adresa funkcie v tabulke symbolov ak existuje, inak skonci program s chybou.
bst_node_t* checkDefined(token_t* var, bst_node_t* symtable)
{
    bst_node_t* result = bst_search(symtable, var->value);
    if(var->type == ID_variable && result == NULL)
    {
        fprintf(stderr,"Semantic analysis error: Variable %s is not defined.\n", var->value);
        exit(5);
    }
    else if(var->type == ID_function && result == NULL)
    {
        fprintf(stderr,"Semantic analysis error: Function %s is not defined.\n", var->value);
        exit(3);
    }
    else
        return result;
}
/// @brief Porovna ci sa parameter rovna s tym v tabulke symbolov.
/// @param fundata Data funkcie z tabulky symbolov.
/// @param index Index parametru.
/// @param type Typ parametra.
/// @return 
bool checkParam(funData_t* fundata, int index, int type)
{
    if(fundata->paramTypes[index-1] == symQInt || fundata->paramTypes[index-1] == symQFloat
       || fundata->paramTypes[index-1] == symQString)
    {
        if(type == funnull)
            return true;
        else if(fundata->paramTypes[index-1] - 3 == type)
            return true;
        else
            return false;
    }
    else if(fundata->paramTypes[index-1] == type)
        return true;
    else
        return false;
}
