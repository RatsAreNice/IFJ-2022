#ifndef IAL_BTREE_TEST_UTIL_H
#define IAL_BTREE_TEST_UTIL_H

#include "ASS.h"
#include <stdio.h>

// Z test util druheho projektu IAL2. Autor: Michal Koutensky

typedef enum direction { left, right, none } direction_t;

char *make_prefix(char *prefix, const char *suffix);

void bst_print_subtree(ASSnode_t *tree, char *prefix, direction_t from);
void PRINTTREEFAST(ASSnode_t *tree);
#endif