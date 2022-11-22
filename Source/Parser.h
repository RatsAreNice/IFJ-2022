#ifndef PARSER_H
#define PARSER_H
#include <stdbool.h>
#include "scanner.h"
bool parse();
bool p_start(token_t * token);
bool p_prolog(token_t * token);
bool p_prolog_sub1(token_t * token);
bool p_prolog_sub2(token_t * token);
bool p_body(token_t * token);
bool p_fparams(token_t * token);
bool p_nparam(token_t * token);
bool p_rettype(token_t * token);
bool p_type(token_t * token);
bool p_nodefbody(token_t * token);
bool p_ifstat(token_t * token);
bool p_whilestat(token_t * token);
bool p_assigned(token_t * token);
bool p_const(token_t * token);
bool p_fcall(token_t * token);
bool p_callargs(token_t * token);
bool p_ncallargs(token_t * token);
bool p_retval(token_t * token);
bool p_vals(token_t * token);
#endif