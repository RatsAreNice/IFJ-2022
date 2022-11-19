/*
BVS VARIANTA SYMTABLU 
KOLABORATORI: Matus Dobias

JADRO PREBRANE Z VYPRACOVANIA 2. DOMACEJ ULOHY PREDMETU IAL MATUSA DOBIASA
*/


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Uzol stromu
typedef struct bst_node {
  char* key;               // kľúč
  int type;              // typ
  funData_t* funData;
  labelData_t* labelData;
  struct bst_node *left;  // ľavý potomok
  struct bst_node *right; // pravý potomok
} bst_node_t;

// data

typedef struct funData
{
  int ParamCount;
  int* paramtypes;
  int returnType;
  bool defined;

} funData_t;



typedef struct labelData
{
  bool declared;
} labelData_t;















// functions

void bst_init(bst_node_t **tree) {
  *tree=NULL;
}

bst_node_t* bst_search(bst_node_t *tree, char* key) {
  if (tree==NULL)
  {
    return NULL;
  }
  else
  {
    int diff = strcmp(tree->key,key);
    if (diff==0)
    {
      
      return tree;
    } else if (diff>0)
    {
      return bst_search(tree->left,key);
    }else
    {
      return bst_search(tree->right,key);
    }
  }
  
  return false;
}

