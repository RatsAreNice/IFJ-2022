#ifndef PARSER_H
#include <stdbool.h>
bool parse();
bool p_start();
bool p_prolog();
bool p_body();
bool p_fparams();
bool p_nparam();
bool p_rettype();
bool p_type();
bool p_nodefbody();
bool p_ifstat();
bool p_whilestat();
bool p_assigned();
bool p_const();
bool p_fcall();
bool p_callargs();
bool p_ncallargs();
bool p_retval();
bool p_vals();
#define PARSER_H
#else
#endif