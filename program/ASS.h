#ifndef ASS_H
#define ASS_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

#define VAR (token_type)-1
#define TOK_PATH(x) x->Patrick_Bateman
#define LEAFCHECK if(node->left->leaf==false){helpsolve(node->left);}if (node->right->leaf==false){helpsolve(node->right);}
// ZISK RELEVANTNYCH POINTEROV
#define GETTHEM char* str1 = checkvar(node->left); char* str2 = checkvar(node->right); 
// FREE NA RELEVANTNE POINTERI
#define FREETHEM free(str1);free(str2);

#define FREESONS delete_node(node->left);delete_node(node->right);

typedef enum operand{
    ADD,
    SUB,
    MUL,
    DIV,
    IDIV,
    LT,
    GT,
    EQ,
    AND,
    OR,
    NOT,
    CONCAT,
    ASSIGN,
    IF,
    THEN,
    LEAF,

}operand_t;


typedef struct ASSnode
{
    bool leaf; 
    bool isvar;
    operand_t OP;
    token_t* Patrick_Bateman;
    struct ASSnode* parent;
    struct ASSnode* left; // left foot
    struct ASSnode* right; // right foot
    //                                          levitating
}ASSnode_t;





void ASSinit(ASSnode_t** tree);
ASSnode_t* makeTree(operand_t OP, ASSnode_t* left, ASSnode_t* right);
ASSnode_t* makeLeaf(token_t* Patrick_Bateman);
void delete_node(ASSnode_t* node);

//non-operand functions
char* checkvar(ASSnode_t* node);
void floatify(token_t* );
void helpsolve(ASSnode_t* node);
void print_code(ASSnode_t** tree);
char* createVar();

// ADD
void ADDInt_Int(ASSnode_t* node);
void ADDFloat_Int(ASSnode_t* node);
void ADDFloat_Float(ASSnode_t* node);

// SUB
void SUBInt_Int(ASSnode_t* node);
void SUBFloat_Int(ASSnode_t* node);
void SUBFloat_Float(ASSnode_t* node);

// ASSIGN
void ASSIGNVAR(ASSnode_t* node);
void ASSIGNVARInt_Float(ASSnode_t* node);
void ASSIGNVARFloat_Int(ASSnode_t* node);
#endif