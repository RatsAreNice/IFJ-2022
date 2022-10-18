// Nazov projektu: IFJ22
// Autor: Richard Blažo (xblazo00)
#ifndef SEMANTIC_H
#define SEMANTIC_H

bst_node_t* checkDefined(token_t* var, bst_node_t* symtable);
bool isDefined(token_t* var, bst_node_t* symtable);
bool checkParam(funData_t* fundata, int paramcount, int type);
#endif