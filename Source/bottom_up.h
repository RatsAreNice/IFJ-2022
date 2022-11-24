#ifndef BOTTOM_UP_H
#define BOTTOM_UP_H
#include "scanner.h"
#include "ASS.h"
int convert(token_t a);
int get_input(token_type ** first, token_type ** second, token_t ** token, token_type end, token_type end2,int skip,int* bracketcount);
int cmp_to_rule(int rs[]);
int expr(token_t* first,token_t* second,token_type end, token_type end2, int skip);
#endif