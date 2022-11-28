//Abstraktny Syntakticky Strom
//autor: Matus Dobias

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
    temp->isvar=false;
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
    temp->isvar=false;
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
    printf(".IFJcode22\n");
    printf("CREATEFRAME\nPUSHFRAME\n");
    helpsolve(*tree);
    if ((*tree)->isvar==true)
    {
        free(TOK_PATH((*tree))->value);
    }
    
    free(*tree);
}

void ADDInt_Int(ASSnode_t* node){
    // code for elementary int + int
    LEAFCHECK // macro
    
    char* var = createVar();
    printf("DEFVAR LF@%s\n",var);
    GETTHEM
    printf("ADD LF@%s %s %s\n",var,str1,str2);
    FREETHEM
    node->Patrick_Bateman=node->left->Patrick_Bateman;
    node->Patrick_Bateman->type=integer;
    if (node->left->isvar) free(TOK_PATH(node->left)->value); 
    if (node->right->isvar) free(TOK_PATH(node->right)->value);
    node->isvar=true;
    node->Patrick_Bateman->value=var;
    node->leaf=true;
    FREESONS
}


void ADDFloat_Int(ASSnode_t* node){
    // code for elementary int + float
    LEAFCHECK // macro

    char* var = createVar();
    printf("DEFVAR LF@%s\n",var);
    if (node->left->Patrick_Bateman->type==integer)
    {
        char* tempvar = createVar();
        printf("DEFVAR LF@%s\n",tempvar);
        char* str1=checkvar(node->left);
        printf("INT2FLOAT LF@%s %s\n",tempvar,str1);
        free(str1);
        node->left->Patrick_Bateman->value=tempvar;
        node->left->Patrick_Bateman->type=ffloat;
    }else{ // 
        char* tempvar = createVar();
        printf("DEFVAR LF@%s\n",tempvar);
        char* str1=checkvar(node->left);
        printf("INT2FLOAT LF@%s %s\n",tempvar,str1);
        free(str1);
        node->right->Patrick_Bateman->value=tempvar;
        node->right->Patrick_Bateman->type=ffloat;
    }
    GETTHEM
    printf("ADD %s %s %s\n",var,str1,str2);
    FREETHEM
    node->Patrick_Bateman=node->left->Patrick_Bateman;
    node->Patrick_Bateman->type=ffloat;
    if (node->left->isvar) free(TOK_PATH(node->left)->value); 
    if (node->right->isvar) free(TOK_PATH(node->right)->value);
    node->isvar=true;
    node->Patrick_Bateman->value=var;
    node->leaf=true;
    FREESONS
}

void ADDFloat_Float(ASSnode_t* node){
    // code for elementary float + float
    LEAFCHECK // macro

    char* var = createVar();
    printf("DEFVAR LF@%s\n",var);
    GETTHEM
    printf("ADD LF@%s %s %s\n",var,str1,str2);
    FREETHEM
    node->Patrick_Bateman=node->left->Patrick_Bateman;
    node->Patrick_Bateman->type=ffloat;
    if (node->left->isvar) free(TOK_PATH(node->left)->value); 
    if (node->right->isvar) free(TOK_PATH(node->right)->value);
    node->isvar=true;
    node->Patrick_Bateman->value=var;
    node->leaf=true;
    FREESONS
}

void SUBInt_Int(ASSnode_t* node){
    // code for elementary int - int
    LEAFCHECK // macro

    char* var = createVar();
    printf("DEFVAR LF@%s\n",var);
    GETTHEM
    printf("SUB LF@%s %s %s\n",var,str1,str2);
    FREETHEM
    node->Patrick_Bateman=node->left->Patrick_Bateman;
    node->Patrick_Bateman->type=integer;
    if (node->left->isvar) free(TOK_PATH(node->left)->value); 
    if (node->right->isvar) free(TOK_PATH(node->right)->value);
    node->isvar=true;
    node->Patrick_Bateman->value=var;
    node->leaf=true;
    FREESONS
}

void SUBFloat_Int(ASSnode_t* node){
    // code for elementary int - float
    LEAFCHECK // macro

    char* var = createVar();
    printf("DEFVAR LF@%s\n",var);
    if (node->left->Patrick_Bateman->type==integer)
    {
        char* tempvar = createVar();
        printf("DEFVAR LF@%s\n",tempvar);
        char* str1=checkvar(node->left);
        printf("INT2FLOAT LF@%s %s\n",tempvar,str1);
        free(str1);
        node->left->Patrick_Bateman->value=tempvar;
        node->left->Patrick_Bateman->type=ffloat;
    }else{ // 
        char* tempvar = createVar();
        printf("DEFVAR LF@%s\n",tempvar);
        char* str1=checkvar(node->left);
        printf("INT2FLOAT %s %s\n",tempvar,str1);
        free(str1);
        node->right->Patrick_Bateman->value=tempvar;
        node->right->Patrick_Bateman->type=ffloat;
    }
    GETTHEM
    printf("SUB LF@%s %s %s\n",var,str1,str2);
    FREETHEM
    node->Patrick_Bateman=node->left->Patrick_Bateman;
    node->Patrick_Bateman->type=ffloat;
    if (node->left->isvar) free(TOK_PATH(node->left)->value); 
    if (node->right->isvar) free(TOK_PATH(node->right)->value);
    node->isvar=true;
    node->Patrick_Bateman->value=var;
    node->leaf=true;
    FREESONS
}

void SUBFloat_Float(ASSnode_t* node){
    // code for elementary float - float
    LEAFCHECK // macro

    char* var = createVar();
    printf("DEFVAR LF@%s\n",var);
    GETTHEM 
    printf("SUB LF@%s %s %s\n",var,str1,str2);
    FREETHEM 
    node->Patrick_Bateman=node->left->Patrick_Bateman;
    node->Patrick_Bateman->type=ffloat;
    if (node->left->isvar) free(TOK_PATH(node->left)->value); 
    if (node->right->isvar) free(TOK_PATH(node->right)->value);
    node->isvar=true;
    node->Patrick_Bateman->value=var;
    node->leaf=true;
    FREESONS
}

void ASSIGNVAR(ASSnode_t* node){
    char* var = malloc(sizeof(char)*strlen(TOK_PATH(node)->value));

    strcpy(var,TOK_PATH(node)->value);
    TOK_PATH(node)->value=var; //CHCEM SA UISTIT ZE TAM BUDE MALLOC

    printf("DEFVAR LF@%s\n",TOK_PATH(node)->value);
    TOK_PATH(node)->type=TOK_PATH(node->left)->type;
    node->leaf=true;
    node->isvar=true;
    char* str1 = checkvar(node);
    char* str2 = checkvar(node->left);
    printf("MOVE %s %s\n",str1,str2);
    FREETHEM
    delete_node(node->left);
}

void ASSIGNVARInt_Float(ASSnode_t* node){
    char* tempvar = createVar();
    printf("DEFVAR LF@%s\n",tempvar);
    char* str1=checkvar(node->left);
    printf("FLOAT2INT LF@%s %s\n",tempvar,str1);
    free(str1);
    if(node->left->isvar) free(TOK_PATH(node->left)->value);
    TOK_PATH(node->left)->value=tempvar;
    TOK_PATH(node->left)->type=integer;
    ASSIGNVAR(node);
    
}
void ASSIGNVARFloat_Int(ASSnode_t* node){
    char* tempvar = createVar();
    printf("DEFVAR LF@%s\n",tempvar);
    char* str1=checkvar(node->left);
    printf("INT2FLOAT LF@%s %s\n",tempvar,str1);
    free(str1);
    if(node->left->isvar) free(TOK_PATH(node->left)->value);
    TOK_PATH(node->left)->value=tempvar;
    TOK_PATH(node->left)->type=integer;
    ASSIGNVAR(node);
    
    
}


void helpsolve(ASSnode_t* node){

    switch (node->OP)
    {
    case ADD:
       LEAFCHECK // macro
       if (node->left->leaf==true && node->right->leaf==true)
       {
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

       } 
       
       break;
       
    case SUB:
        LEAFCHECK // macro
        if (node->left->leaf==true && node->right->leaf==true)
       {
            if (node->TOK_PATH(left)->type==ffloat && node->TOK_PATH(right)->type==ffloat )
            {
                SUBFloat_Float(node);
            } else if (node->TOK_PATH(left)->type==integer && node->TOK_PATH(right)->type==integer)
            {
                SUBInt_Int(node);
            }
            else
            {
                SUBFloat_Int(node);
            }
            break;
       } 
       break;
    case ASSIGN:
        if (node->left->leaf==false) helpsolve(node->left);
        if (node->left->leaf==true)
        {
            if (TOK_PATH(node->left)->type==ID_variable)
            {
                ASSIGNVAR(node);
                break;
            }
            if (TOK_PATH(node->left)->type == TOK_PATH(node)->type)
            {
                ASSIGNVAR(node);
                break;
            } else if (TOK_PATH(node)->type==ffloat && TOK_PATH(node->left)->type==integer)
            {
                ASSIGNVARFloat_Int(node);
                break;
            } else if (TOK_PATH(node)->type==ffloat && TOK_PATH(node->left)->type==integer)
            {
                ASSIGNVARFloat_Int(node);
                break;
            }
            
            
        }
        
        
        
    default:
        fprintf(stderr,"UNEXPECTED OPERAND HOW DID THAT HAPPEN\n");
        exit(7);
    }

}

void floatify(token_t* Patrick_Bateman){
    sprintf(Patrick_Bateman->value,"%a",strtof(Patrick_Bateman->value,NULL));
}

char* checkvar(ASSnode_t* node){
    
    char* strptr=malloc(sizeof(char)*20);
    if (node->isvar)
    {
        sprintf(strptr,"LF@%s",TOK_PATH(node)->value);
        return strptr;
    }else{
        switch (TOK_PATH(node)->type)
        {
        case integer:
            sprintf(strptr,"int@%s",TOK_PATH(node)->value);
            return strptr;
        case ffloat:
            floatify(TOK_PATH(node));
            sprintf(strptr,"float@%s",TOK_PATH(node)->value);
            return strptr;
        case string:
            sprintf(strptr,"string@%s",TOK_PATH(node)->value);
            return strptr;
        
        default:
            fprintf(stderr,"JIHAD JIHAD JIHAD\n");
            exit(17);
           
        }
    }
}

char* createVar(){
    static unsigned int varcount;
    char* str = malloc(sizeof(char)*14); // var + maxuint length + \0 
    sprintf(str,"var%d",varcount);
    varcount++;
    return str;
}