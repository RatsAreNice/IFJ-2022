#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symtable.h"
#include "scanner.h"

bst_node_t* isDefined(token_t* var, bst_node_t* symtable)
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
