#ifndef PARSER_H
#define PARSER_H
#include <stdbool.h>
#include "scanner.h"
#include "symdll.h"
#include "bottom_up.h"

bool parse();

bool p_start(token_t * token, ASSnode_t **astree);

bool p_prolog(token_t * token);
bool p_prolog_sub1(token_t * token);
bool p_prolog_sub2(token_t * token);

bool p_body(token_t * token, bool defallowed, ASSnode_t *astree, bool mainreturn, returnType_t rettyp);


bool p_fundec(token_t * token, ASSnode_t *astree);
bool p_funargs(token_t * token, ASSnode_t *astree, funData_t* newfunc);
bool p_funbody(token_t * token, ASSnode_t *astree, funData_t* newfunc);

bool p_fparams(token_t * token, ASSnode_t *astree, funData_t* newfunc);
bool p_nparam(token_t *token, ASSnode_t *astree, funData_t* newfunc);

bool p_ifstat(token_t * token, ASSnode_t *astree, bool mainreturn, returnType_t rettyp);
bool p_ifbody(token_t *token, ASSnode_t *astree, bool mainreturn, returnType_t rettyp);
bool p_elsebody(token_t *token, ASSnode_t *astree, bool mainreturn, returnType_t rettyp);

bool p_whilestat(token_t * token, ASSnode_t *astree, bool mainreturn, returnType_t rettyp);
bool p_whilebody(token_t * token, ASSnode_t *astree, bool mainreturn, returnType_t rettyp);

bool p_assigned(token_t *token, ASSnode_t *astree);
bool p_const(token_t * token, token_type expect1, token_type expect2);
bool p_fcall(token_t *token, ASSnode_t *astree);
bool p_callargs(token_t *token, ASSnode_t *astree, bst_node_t *symtableentry, int* paramcount);
bool p_ncallargs(token_t *token, ASSnode_t *astree, bst_node_t *symtableentry, int* paramcount);
bool p_vals(token_t *token, ASSnode_t* astree, bst_node_t *symtableentry, int* paramcount);
#endif