#ifndef BOTTOM_UP_H
#define BOTTOM_UP_H
#include "scanner.h"
int convert(token_t a);
int get_input(token_t end,int skip);
int cmp_to_rule(int rs[]);
int expr(token_t end, int skip);
#endif