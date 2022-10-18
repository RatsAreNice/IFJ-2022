/*
BVS VARIANTA SYMTABLU 
KOLABORATORI: Matus Dobias

JADRO PREBRANE Z VYPRACOVANIA 2. DOMACEJ ULOHY PREDMETU IAL MATUSA DOBIASA
*/


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"



/// @brief Inicializacia koren
/// @param tree koren
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
  
  return NULL;
}



/// @brief Vklada a replacuje uzly v tabulke symbolov
/// @param tree koren
/// @param key kluc uzlu
/// @param type typ uzlu
/// @param funData odkaz na informacie o funkcii
/// @return RETRUNS 1 -> SUCCESSFUL INSERT RETURNS 2 -> SUCCESFUL REPLACE
int bst_insert(bst_node_t **tree, char* key, int type, funData_t* funData) {
  int diff = 0;
  if (*tree!=NULL)
  {
    diff = strcmp((*tree)->key,key);
  }
  // naplast solution, nechcelo sa mi refaktorizovat
  if (*tree==NULL)
  {
    bst_node_t* temp = (bst_node_t*)malloc(sizeof(struct bst_node));
    if (temp==NULL)
    {
      return 99; //MALLOC FAIL
    }
    temp->type=type;
    temp->funData=funData;
    temp->key=key;
    temp->left=NULL;
    temp->right=NULL;
    
    *tree=temp;
    return 1;
  }
  else if (diff>0)
  {
    return bst_insert(&(*tree)->left,key,type,funData);
  }
  else if (diff<0)
  {
    return bst_insert(&(*tree)->right,key,type,funData);
  }
  else
  {
    (*tree)->type=type;
    (*tree)->funData=funData;
    return 2;
  }

}

/// @brief prelozi typ tokenu na presnejsi typ
/// @param tokenvalue string obsahujuci typ tokenu
/// @return vrati typ returnu pre funkciu
returnType_t translate(char* tokenvalue)
{

  if(strcmp(tokenvalue, "int") == 0)
      return symInt;
  else if(strcmp(tokenvalue, "float") == 0)
      return symFloat;
  else if(strcmp(tokenvalue, "string") == 0)
      return symString;
  else if(strcmp(tokenvalue, "?float") == 0)
      return symQFloat;
  else if(strcmp(tokenvalue, "?int") == 0)
      return symQInt;
  else if(strcmp(tokenvalue, "?string") == 0)
      return symQString;
  else
      return symVoid;
}

/// @brief Pomocna funkcia pre delete funckiu
/// @param target destinacia premiestnenia
/// @param tree uzol stromu
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if ((*tree)->right->right!=NULL)
  {
    bst_replace_by_rightmost(target,&(*tree)->right);
  }else
  //(*tree)->right je ptr
  //(*tree) je previous
  {
    target->key=(*tree)->right->key;  //target kluc nahradeny rightmost klucom
    target->type=(*tree)->right->type; //target value nahradeny rightmost value
    funData_t* temp2 = target->funData;
    target->funData=(*tree)->right->funData;
    bst_node_t* temp=(*tree)->right;
    (*tree)->right=(*tree)->right->left; // otec rightmost uzlu zdedil lavu hranu svojho syna
    free(temp2);
    free(temp); 
  }
}


/// @brief Odstranenie uzlu zo stromu
/// @param tree koren
/// @param key kluc odstranovaneho uzlu
void bst_delete(bst_node_t **tree, char* key) {
  int diff = 0;
  if ((*tree)!=NULL)
  {
    diff = strcmp((*tree)->key,key);
  }
  
  if ((*tree)==NULL)
  {
    
    return;
  } // prazdny koren
  if (diff==0) //mazany kluc je v koreni alebo ma 2 synov
  {
    
    bst_replace_by_rightmost(*tree,&(*tree)->left);
    return;
  }
  if (((*tree)->left == NULL && diff<0 )||((*tree)->right == NULL && diff>0 ))
  {
    //node doesnt exist
    return;
  }
  
  if ((*tree)->left!=NULL) // zistujem ci v lavo nieje hladany uzol
  {
    int diff2 = strcmp((*tree)->left->key,key);
    if (diff2 == 0)
    {
      
      if ((*tree)->left->left!=NULL && (*tree)->left->right!=NULL)
      {
        bst_delete(&(*tree)->left,key);
        return;
      }else
      {
        // mazany node ma 0 alebo jeden poduzol
        bst_node_t* temp=(*tree)->left;
        if ((*tree)->left->left!=NULL)
        {
          (*tree)->left=temp->left;
        }else
        {
          (*tree)->left=temp->right;
        }
        free(temp->funData);
        free(temp); 
        return;
      }
    }
    
  }
  if ((*tree)->right!=NULL)
  {
    int diff2 = strcmp((*tree)->left->key,key);
    if (diff2 == 0)
    {
      
      if ((*tree)->right->left!=NULL && (*tree)->right->right!=NULL)
      {
        bst_delete(&(*tree)->right,key);
        return;
      }else
      {
        // mazany node ma 0 alebo jeden poduzol
        bst_node_t* temp=(*tree)->right;
        if ((*tree)->right->left!=NULL)
        {
          (*tree)->right=temp->left;
        }else
        {
          (*tree)->right=temp->right;
        }
        free(temp->funData);
        free(temp); 
        return;
      }
    }
  }

  
  if (diff<0)
  {
    bst_delete(&(*tree)->left,key);
    return;
  } else
  {
    bst_delete(&(*tree)->right,key);
    return;
  }

  return;
}

/// @brief Navratenie stavu stromu do momentu po inicializacii
/// @param tree koren stromu
void bst_dispose(bst_node_t **tree) {
  
  if ((*tree)!=NULL)
  {
    bst_dispose(&(*tree)->left);
    bst_dispose(&(*tree)->right);
    (*tree)->left=NULL;
    (*tree)->right=NULL;
    free((*tree)->funData);
    free(*tree); // 116 -> 220
  }
  *tree=NULL;

}



