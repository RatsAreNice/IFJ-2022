#ifndef PARSER_H
#define PARSER_H
#include <stdbool.h>
#include "scanner.h"
#include "bottom_up.h"

bool parse();

bool p_start(token_t * token);

bool p_prolog(token_t * token);
bool p_prolog_sub1(token_t * token);
bool p_prolog_sub2(token_t * token);

bool p_body(token_t * token, bool defallowed);

bool p_fundec(token_t * token);
bool p_funargs(token_t * token);
bool p_funbody(token_t * token);
bool p_fparams(token_t * token);


bool p_fparams(token_t * token);
bool p_nparam(token_t * token);

//bool p_expr(token_t * token);

bool p_rettype(token_t * token);
bool p_type(token_t * token);

bool p_nodefbody(token_t * token);

bool p_ifstat(token_t * token);
bool p_ifbody(token_t *token);
bool p_elsebody(token_t *token);

bool p_whilestat(token_t * token);
bool p_whilebody(token_t * token);

bool p_assigned(token_t * token);
bool p_const(token_t * token, token_type expect1, token_type expect2);
bool p_fcall(token_t * token);
bool p_callargs(token_t * token);
bool p_ncallargs(token_t * token);
bool p_retval(token_t * token);
bool p_vals(token_t * token);
#endif