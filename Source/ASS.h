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

void print_code(ASSnode_t** tree);
char* createVar();
void ADDInt_Int(ASSnode_t* node);





#endif