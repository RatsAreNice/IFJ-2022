#ifndef SYMTABLE_H
#define SYMTABLE_H
#include <stdbool.h>
#include "scanner.h"

typedef struct funData funData_t;
typedef struct bst_node bst_node_t;

void bst_init(bst_node_t **tree);

typedef enum returnType{
    symInt = integer,
    symString = string,
    symFloat = ffloat,
    symQInt, // ?int type -> moze obsahovat null
    symQString, // ?string type -> moze obsahovat null
    symQFloat, // ?float type -> moze obsahovat null
    symVoid = funvoid // nic nevracia
} returnType_t;

typedef struct dependencyPair
{
  char* functionid;
  int paramCount;

} dependencyPair_t;

typedef struct funData
{
  int ParamCount;
  int* paramTypes;
  int returnType;
  int depCount;
  dependencyPair_t** dependencies;
  bool defined;

} funData_t;

// Uzol stromu
typedef struct bst_node {
  char* key;               
  int type;              
  funData_t* funData; // data funkcie
  //labelData_t* labelData;
  struct bst_node *left;  
  struct bst_node *right; 
} bst_node_t;

returnType_t translate(char* tokenvalue);

bst_node_t* bst_search(bst_node_t *tree, char* key);

int bst_insert(bst_node_t **tree, char* key, int type, funData_t* funData);

void bst_dispose(bst_node_t **tree);
void bst_delete(bst_node_t **tree, char* key);
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree);
#endif