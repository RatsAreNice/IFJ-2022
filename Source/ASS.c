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
    // ASS -> ??? -> kod
}

void ADDInt_Int(ASSnode_t* node){
    // code for elementary int + int
    if (node->left->leaf==false)
    {
        helpsolve(node->left);
    }
    if (node->right->leaf==false)
    {
        helpsolve(node->right);
    }
    
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

void ADDFloat_Float(ASSnode_t* node){
    // code for elementary float + float
    char* var = createVar();
    //printf("DEFVAR %s\n",var);

    //printf("ADD %s float@%s float@%s\n",var,node->left->Patrick_Bateman->value,node->right->Patrick_Bateman->value);
    node->Patrick_Bateman=node->left->Patrick_Bateman;
    node->Patrick_Bateman->type=ffloat;
    node->Patrick_Bateman->value=var;
    node->leaf=true;
}

void SUBInt_Int(ASSnode_t* node){
    // code for elementary int - int
    char* var = createVar();
    printf("DEFVAR %s\n",var);
    printf("SUB %s int@%s int@%s\n",var,node->left->Patrick_Bateman->value,node->right->Patrick_Bateman->value);
    node->Patrick_Bateman=node->left->Patrick_Bateman;
    node->Patrick_Bateman->type=integer;
    node->Patrick_Bateman->value=var;
    node->leaf=true;
}

void SUBFloat_Int(ASSnode_t* node){
    // code for elementary int - float
    char* var = createVar();
    printf("DEFVAR %s\n",var);
    if (node->left->Patrick_Bateman->type==integer)
    {
        char* tempvar = createVar();
        printf("DEFVAR %s\n",tempvar);
        printf("INT2FLOAT %s int@%s",tempvar,node->left->Patrick_Bateman->value);
        node->left->Patrick_Bateman->value=tempvar;
        node->left->Patrick_Bateman->type=ffloat;
    }else{ // 
        char* tempvar = createVar();
        printf("DEFVAR %s\n",tempvar);
        printf("INT2FLOAT %s int@%s",tempvar,node->right->Patrick_Bateman->value);
        node->right->Patrick_Bateman->value=tempvar;
        node->right->Patrick_Bateman->type=ffloat;
    }
    
    printf("SUB %s float@%s float@%s\n",var,node->left->Patrick_Bateman->value,node->right->Patrick_Bateman->value);
    node->Patrick_Bateman=node->left->Patrick_Bateman;
    node->Patrick_Bateman->type=ffloat;
    node->Patrick_Bateman->value=var;
    node->leaf=true;
}

void SUBFloat_Float(ASSnode_t* node){
    // code for elementary float - float
    char* var = createVar();
    printf("DEFVAR %s\n",var);
    printf("SUB %s float@%s float@%s\n",var,node->left->Patrick_Bateman->value,node->right->Patrick_Bateman->value);
    node->Patrick_Bateman=node->left->Patrick_Bateman;
    node->Patrick_Bateman->type=ffloat;
    node->Patrick_Bateman->value=var;
    node->leaf=true;
}

void helpsolve(ASSnode_t* node){

    switch (node->OP)
    {
    case ADD:
        if (node->left->Patrick_Bateman->type==ffloat && node->right->Patrick_Bateman->type==ffloat )
        {
            ADDFloat_Float(node);
        } else if (node->left->Patrick_Bateman->type==integer && node->right->Patrick_Bateman->type==integer)
        {
            ADDInt_Int(node);
        }
        else
        {
            ADDFloat_Int(node);
        }
        break;
    case SUB:
        if (node->left->Patrick_Bateman->type==ffloat && node->right->Patrick_Bateman->type==ffloat )
        {
            SUBFloat_Float(node);
        } else if (node->left->Patrick_Bateman->type==integer && node->right->Patrick_Bateman->type==integer)
        {
            SUBInt_Int(node);
        }
        else
        {
            SUBFloat_Int(node);
        }
        break;
    default:
        fprintf(stderr,"UNEXPECTED OPERAND HOW DID THAT HAPPEN\n");
        break;
    }

}

char* createVar(){
    static unsigned int varcount;
    char* str = malloc(sizeof(char)*14); // var + maxuint length + \0 
    //sprintf(str,"var%d",varcount);
    varcount++;
    return str;
}