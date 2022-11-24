#include "ASS.h"

void ASSinit(ASSnode_t** tree){
    *tree = NULL;
}

ASSnode_t* makeTree(operand_t OP, ASSnode_t* left, ASSnode_t* right){
    ASSnode_t* temp = malloc(sizeof(struct ASSnode));
    if (temp==NULL)
    {
        exit(99);
    }
    temp->leaf=false;
    temp->left=left;
    temp->right=right;
    temp->parent=NULL;
    temp->OP=OP;
    temp->Patrick_Bateman=NULL;

    return temp;
}

ASSnode_t* makeLeaf(token_t* Patrick_Bateman){
    ASSnode_t* temp = malloc(sizeof(struct ASSnode));
    if (temp==NULL)
    {
        exit(99);
    }
    temp->leaf=true;
    temp->left=NULL;
    temp->right=NULL;
    temp->parent=NULL;
    temp->OP=LEAF;
    temp->Patrick_Bateman=Patrick_Bateman;

    return temp;
}
