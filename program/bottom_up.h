#ifndef BOTTOM_UP_H
#define BOTTOM_UP_H
#include "scanner.h"
#include "ASS.h"
#include "c206.h"
#include "Parser.h"

int convert(token_t a);
unit_t get_input(token_type ** first,char* firstv, token_type ** second,char*secondv, token_t ** token, token_type end, token_type end2,int skip,int* bracketcount);
unit_t cmp_to_rule(unit_t rs[]);
ASSnode_t* expr(token_t* first,token_t* second,token_type end, token_type end2, int skip);
#endif