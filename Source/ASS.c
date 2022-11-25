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

void delete_node(ASSnode_t* node){
    free(node);
}





void print_code(ASSnode_t** tree){
    switch ((*tree)->OP)
    {
    case ADD:
        /* code */
        break;
    
    default:
        break;
    }
}

void ADDInt_Int(ASSnode_t* node){
    // code for elementary int + int
    char* var = createVar();
    //printf("DEFVAR %s\n",var);
    //printf("ADD %s %s %s\n",var,node->left->Patrick_Bateman->value,node->right->Patrick_Bateman->value);
    node->Patrick_Bateman=node->left->Patrick_Bateman;
    node->Patrick_Bateman->type=integer;
    node->Patrick_Bateman->value=var;
    node->leaf=true;
}

//void ADDInt_Int(ASSnode_t* node){
//    // code for elementary int + int
//    char* var = createVar();
//    printf("DEFVAR %s\n",var);
//    printf("ADD %s int@%s int@%s\n",var,node->left->Patrick_Bateman->value,node->right->Patrick_Bateman->value);
//    node->Patrick_Bateman=node->left->Patrick_Bateman;
//    node->Patrick_Bateman->type=integer;
//    node->Patrick_Bateman->value=var;
//    node->leaf=true;
//}

void ADDFloat_Int(ASSnode_t* node){
    // code for elementary int + int
    char* var = createVar();
    //printf("DEFVAR %s\n",var);

    //printf("ADD %s float@%s float@%s\n",var,node->left->Patrick_Bateman->value,node->right->Patrick_Bateman->value);
    node->Patrick_Bateman=node->left->Patrick_Bateman;
    node->Patrick_Bateman->type=integer;
    node->Patrick_Bateman->value=var;
    node->leaf=true;
}

char* createVar(){
    static unsigned int varcount;
    char* str = malloc(sizeof(char)*14); // var + maxuint length + \0 
    //sprintf(str,"var%d",varcount);
    varcount++;
    return str;
}