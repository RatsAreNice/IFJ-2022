#ifndef ASS_H
#define ASS_H
#include "scanner.h"

typedef enum{
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


}operand;




typedef struct ASSnode
{
    bool leaf; // ???
    operand OP;
    token_t* Patrick_Bateman;
    ASSnode_t* parent;
    ASSnode_t* left;
    ASSnode_t* rightt;
}ASSnode_t;





void ASSinit(ASSnode_t** tree);
bool makeTree(operand OP, ASSnode_t* left, ASSnode_t* right);



#endif