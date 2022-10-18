// Abstraktny Syntakticky Strom
// projekt: IFJ22
/// @authors: Matus Dobias
#ifndef ASS_H
#define ASS_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"

extern FILE *allahprint;
#define VAR (token_type) - 1
#define bbool (token_type) - 2
#define TOK_PATH(x) x->Patrick_Bateman
#define LEAFCHECK                     \
  if (node->left != NULL){            \
    if (node->left->leaf == false) {  \
      helpsolve(node->left);          \
    }                                 \
  }                                   \
  if(node->right != NULL) {         \
    if (node->right->leaf == false) { \
      helpsolve(node->right);         \
    }                                 \
  } 

// ZISK RELEVANTNYCH POINTEROV
#define GETTHEM                      \
  char *str1 = checkvar(node->left); \
  char *str2 = checkvar(node->right);
// FREE NA RELEVANTNE POINTERI
#define FREETHEM \
  free(str1);    \
  free(str2);

#define FREESONS           \
  delete_node(node->left); \
  delete_node(node->right);

typedef enum operand {
  ADD,
  SUB,
  MUL,
  DIV,
  LT,
  GT,
  LTE,
  GTE,
  EQ,
  NEQ,
  AND,
  OR,
  NOT,
  CONCAT,
  ASSIGN,
  IF,
  WHILE,
  THEN,
  LEAF,
  FUNCTIONCALL,
  ARGS,
  RETURN,
  FDEC,
  FDEC_DATA,
  FDEC_DATA_INF,
  FDEC_PARAMS,
  FDEC_NPARAM,
  RYAN_GOSLING

} operand_t;

typedef struct ASSnode {
  bool leaf;
  bool isvar;
  operand_t OP;
  token_t *Patrick_Bateman;
  struct ASSnode *parent;
  struct ASSnode *left;   // left foot
  struct ASSnode *right;  // right foot
                          //                                          levitating
} ASSnode_t;

void ASSinit(ASSnode_t **tree);
ASSnode_t *makeTree(operand_t OP, ASSnode_t *left, ASSnode_t *right);
ASSnode_t *makeLeaf(token_t *Patrick_Bateman);
void delete_node(ASSnode_t *node);

// non-operand functions
char *labelgen();
char *checkvar(ASSnode_t *node);
void floatify(token_t *);
void helpsolve(ASSnode_t *node);
void print_code(ASSnode_t **tree);
char *createVar();
void print_builtins();
void reads();
void readi();
void readf();

// ADD
void ADDInt_Int(ASSnode_t *node);
void ADDFloat_Int(ASSnode_t *node);
void ADDFloat_Float(ASSnode_t *node);

// SUB
void SUBInt_Int(ASSnode_t *node);
void SUBFloat_Int(ASSnode_t *node);
void SUBFloat_Float(ASSnode_t *node);

// ASSIGN
void ASSIGNVAR(ASSnode_t *node);
void ASSIGNVARInt_Float(ASSnode_t *node);
void ASSIGNVARFloat_Int(ASSnode_t *node);

// LT,GT,EQ
void NILCHECK(ASSnode_t *node);
char *CHECKTYPE(ASSnode_t *node);
void LTCOMP(ASSnode_t *node,bool lolec);
void GTCOMP(ASSnode_t *node,bool lolec);

// CONCAT
void CONCATSTR(ASSnode_t *node);

void assprint(ASSnode_t *node);

#endif