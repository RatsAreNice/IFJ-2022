#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symtable.h"
#include "scanner.h"

bool isDefined(token_t* var, bst_node_t* symtable)
{
    bst_node_t* result = bst_search(symtable, var->value);
    if(result == NULL)
        return false;
    else
        return true;
}

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

bool checkParam(funData_t* fundata, int paramcount, int type)
{
    if(fundata->paramTypes[paramcount-1] == symQInt || fundata->paramTypes[paramcount-1] == symQFloat
       || fundata->paramTypes[paramcount-1] == symQString)
    {
        if(type == funnull)
            return true;
        else if(fundata->paramTypes[paramcount-1] - 3 == type)
            return true;
        else
            return false;
    }
    else if(fundata->paramTypes[paramcount-1] == type)
        return true;
    else
        return false;
}
