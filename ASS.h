#ifndef ASS_H
#define ASS_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

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
    bool leaf; // ???
    operand_t OP;
    token_t* Patrick_Bateman;
    struct ASSnode* parent;
    struct ASSnode* left;
    struct ASSnode* right;
}ASSnode_t;





void ASSinit(ASSnode_t** tree);
ASSnode_t* makeTree(operand_t OP, ASSnode_t* left, ASSnode_t* right);
ASSnode_t* makeLeaf(token_t* Patrick_Bateman);
void delete_node(ASSnode_t* node);
//non-operand functions
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

#endif