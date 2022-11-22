// Parser
// Autor: Richard Blažo

/*
parser 'states'?
<start> 
<prolog>
<body>
<fparams>
<nparam>
<rettype>
<type>
<nodefbody>
<ifstat>
<whilestat>
<assigned>
<const>
<fcall>
<callargs>
<ncallargs>
<retval>
<vals>
*/
#define DEBUG
#ifdef DEBUG
# define DPRINT(x) printf x
#else
# define DPRINT(x) do {} while (0)
#endif


#include "scanner.h"
#include "bottom_up.h"
#include "symtable.h"
#include "Parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

typedef enum
{
    NOSKIP,
    SKIP
} scannermode;

bool parse()
{
    // Init symtable a pridat default funkcie.
    token_t token = get_token(1);

    if ( p_start(&token) == false )
    {
        fprintf(stderr, "Unknown syntax error.");
        exit(2);
    }
    return false;
}

bool p_start(token_t * token)
{
    return p_prolog(token) && p_body(token);
    return false;
}

bool p_prolog(token_t * token)
{
    if(p_prolog_sub1(token) && p_prolog_sub2(token))
        return true;
    else
    {
        fprintf(stderr, "Syntax chyba: Unexpected error, last token: %s\n", token->value);
        exit(2);
    }
    return false;
}

bool p_prolog_sub1(token_t * token)
{
    if(token->type == prolog1)
    {
        DPRINT(("Token %s matches prolog_sub_1!\n", token->value));
        *token = get_token(0);
        return true;
    }
    else
    {
        fprintf(stderr, "Syntax chyba: Expected prolog at beginning of file, got: %s\n", token->value);
        exit(2);
    }
    return false;
}

bool p_prolog_sub2(token_t * token)
{
    if(token->type == prolog2)
    {
        DPRINT(("Token %s matches prolog_sub_2!\n", token->value));
        *token = get_token(0);
        return true;
    }
    else
    {
        fprintf(stderr, "Syntax chyba: Malformed prolog at beginning of file, got: %s\n", token->value);
        exit(2);
    }
    return false;
}

bool p_body(token_t * token)
{
    DPRINT(("Got to body with token %s\n", token->value));
    switch(token->type)
    {
        case ffloat:
        case integer:
        case string:
            return p_const(token) && p_body(token);
        case function:
            *token = get_token(0);
        default:
            fprintf(stderr, "Syntax error: Didn't get what expected :(");
    }
    exit(0);
}

bool p_const(token_t * token)
{
    //token_t oldtoken = token;
    token_t dummy;
    dummy.type = semicolon;
    *token = get_token(0);
    if(strcmp(token->value,";") != 0)
    {
        if(expr(dummy,0) == 0)
        {
            DPRINT(("Got here after expr.\n"));
            *token = get_token(0);
            return true;
        }
        else
        {
            fprintf(stderr, "Syntax error: Expected ';' after literal, got %s.\n", token->value);
            exit(2);
        }
    }
    else
    {
        DPRINT(("Got here.\n"));
        *token = get_token(0);
        return true;
    }
}




