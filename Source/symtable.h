#ifndef SYMTABLE_H
#define SYMTABLE_H
#include <stdbool.h>
typedef struct funData funData_t;
typedef struct bst_node bst_node_t;

void bst_init(bst_node_t **tree);

typedef enum {
    symInt,
    symString,
    symFloat,
    symQInt,
    symQString,
    symQFloat,
    symVoid,
} symTypes_t;

typedef struct funData
{
  int ParamCount;
  int* paramTypes;
  int returnType;
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

bst_node_t* bst_search(bst_node_t *tree, char* key);

int bst_insert(bst_node_t **tree, char* key, int type, funData_t* funData);

void bst_dispose(bst_node_t **tree);
void bst_delete(bst_node_t **tree, char* key);
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree);
#endif