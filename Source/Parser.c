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

bool parse();

bool p_start(token_t * token);

bool p_prolog(token_t * token);
bool p_prolog_sub1(token_t * token);
bool p_prolog_sub2(token_t * token);

bool p_body(token_t * token);

bool p_fundec(token_t * token);
bool p_fparams(token_t * token);
bool p_nparam(token_t * token);

//bool p_expr(token_t * token);

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
}

bool p_prolog(token_t * token)
{
    if(p_prolog_sub1(token) && p_prolog_sub2(token))
        return true;
    else
    {
        fprintf(stderr, "Syntax error: Unexpected error, last token: %s\n", token->value);
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
        fprintf(stderr, "Syntax error: Expected prolog at beginning of file, got: %s\n", token->value);
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
        fprintf(stderr, "Syntax error: Malformed prolog at beginning of file, got: %s\n", token->value);
        exit(2);
    }
    return false;
}

bool p_body(token_t * token)
{
    token_t oldtoken;
    DPRINT(("Got to body with token %s\n", token->value));
    switch(token->type)
    {
        case ffloat:
        case integer:
        case string:
            if(p_const(token))
            {
                *token = get_token(0);
                return true && p_body(token);
            }
            else
                
        case ID_variable:
            oldtoken = *token;
            *token = get_token(0);
            if(token->type != assign)
            {
                if (expr(&oldtoken, token, semicolon, 0) == 0)
                {   
                    *token = get_token(0);
                    return true && p_body(token);
                }
                else
                {
                    fprintf(stderr, "Syntax error: Unknown token.\n");
                    exit(2);
                }
            }
            else
            {
                *token = get_token(0);
                if(p_assigned(token))
                {
                    *token = get_token(0);
                    return true && p_body(token);
                }
                else
                {
                    fprintf(stderr, "Syntax error: Unknown token after assignment.\n");
                    exit(2);
                }
            }
        //case function:
        //    *token = get_token(0);
        case funif:
            return p_ifstat(token);
        default:
            fprintf(stderr, "Syntax error: Didn't get what expected :(");
    }
    exit(0);
}

//bool p_expr(token_t * oldtoken, token_t * token)
//{
//
//}

bool p_const(token_t * token)
{
    token_t oldtoken = *token;
    *token = get_token(0);
    if(token->type != semicolon)
    {

        if(expr(&oldtoken, token, semicolon,0) == 0)
        {
            DPRINT(("%s was an expression.\n", oldtoken.value));
            return true;
        }
        else
        {
            fprintf(stderr, "Syntax error: Failed to handle expression.\n");
            exit(2);
        }
    }
    else
    {
        DPRINT(("%s was a constant literal.\n", oldtoken.value));
        return true;
    }
}

bool p_assigned(token_t *token)
{
    if(token->type == ID_variable)
    {
        token_t oldtoken = *token;
        *token = get_token(0);
        if(token->type != semicolon)
        {
            if(expr(&oldtoken, token, semicolon,0) == 0)
            {
                DPRINT(("%s was an expression.\n", oldtoken.value));
                *token = get_token(0);
                return true;
            }
            else
            {
                fprintf(stderr, "Syntax error: Failed to handle expression.\n");
                exit(2);
            }
        }
        else
            return true;
    }
    else if(token->type == ffloat || token->type == integer || token->type == string)
        return p_const(token);
    else if(token->type == ID_function)
    {
        *token = get_token(0);
        return p_fcall(token);
    }
    else
    {
        fprintf(stderr, "Syntax error: Expected a value, a function or variable, got %s\n", token->value);
        exit(2);
    }
}

bool p_fcall(token_t *token)
{
    if(token->type == lbracket)
    {
        *token = get_token(0);
        if(p_callargs(token))
            return true;
        else
        {
            fprintf(stderr, "Syntax error: Failed to parse function call arguments\n");
            exit(2);
        }
    }
    else
    {
        fprintf(stderr, "Syntax error: Expected '(', got %s\n", token->value);
        exit(2);
    }
}

bool p_callargs(token_t *token)
{
    if(token->type == rbracket)
    {
        *token = get_token(0);
        return true;
    }
    else
    {
        fprintf(stderr, "Not implemented yet.\n");
        exit(2);
    }
}

bool p_vals(token_t *token)
{
    return true;
}

bool p_ifstat(token_t *token)
{
    *token = get_token(0);
    if(token->type == lbracket)
    {
        token_t oldtoken = *token;
        *token = get_token(0);
        if(token->type == ID_function)
        {
            fprintf(stderr, "Not implemented yet.\n");
            exit(2);
        }
        else
        {
            if(expr(&oldtoken, token, lsetbracket,0) == 0)
            {
                DPRINT(("%s was an expression in if statement\n", token->value));
                return true;
                //check { -> <nodefbody> until }
            }
            else
            {
                fprintf(stderr, "Syntax error: Failed to handle expression.\n");
                exit(2);
            }
        }
    }
    else
    {
        fprintf(stderr, "Syntax error: Expected '(' after 'if', got '%s'\n", token->value);
        exit(2);
    }
}