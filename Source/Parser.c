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

bool p_body(token_t * token, bool defallowed);

bool p_fundec(token_t * token);
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
    return p_prolog(token) && p_body(token, true);
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

bool p_body(token_t * token, bool defallowed)
{
    token_t oldtoken;
    DPRINT(("Got to body with token %s of type %d\n", token->value, token->type));
    switch(token->type)
    {
        case ID_function:
            DPRINT(("ID FUNCTION %s of type %d\n", token->value, token->type));
            *token = get_token(0);
            if(p_fcall(token))
            {
                *token = get_token(0);
                return p_body(token,defallowed);
            }
            else
            {
                fprintf(stderr, "Syntax error: Function call failed in body.");
                exit(2);
            }
        case ffloat:
        case integer:
        case string:
            if(p_const(token, semicolon, semicolon))
            {
                *token = get_token(0);
                return true && p_body(token, defallowed);
            }
            else
            {
                fprintf(stderr, "Syntax error: Didn't get what expected :(");
                exit(2);
            }
        case ID_variable:
            oldtoken = *token;
            *token = get_token(0);
            if(token->type != assign)
            {
                if (expr(&oldtoken, token, semicolon, semicolon, 0) == 0)
                {   
                    *token = get_token(0);
                    return true && p_body(token, defallowed);
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
                    return true && p_body(token, defallowed);
                }
                else
                {
                    fprintf(stderr, "Syntax error: Unknown token after assignment.\n");
                    exit(2);
                }
            }
        case function:
            if(!defallowed)
            {
                fprintf(stderr, "Syntax error: Can't define function inside if/while/function.\n");
                exit(2);
            }
            else
            {
                *token = get_token(0);
                return p_fundec(token) && p_body(token, defallowed);
            }
        case epilog:
            if(!defallowed)
            {
                fprintf(stderr, "Syntax error: Can't use epilog in the inside of if/while/function.\n");
                exit(2);
            }
            else
            {
                *token = get_token(1);
                if(token->type != eof)
                {
                    fprintf(stderr, "Syntax error: Tokens found after epilog\n");
                    exit(2);
                }
                else
                    return true;
            }
        case funif:
            return p_ifstat(token) && p_body(token, defallowed);
        case funwhile:
            return p_whilestat(token) && p_body(token, defallowed);
        case rsetbracket:
            if(!defallowed)
            {
                *token = get_token(0);
                return true;
            }
            else
            {
                fprintf(stderr, "Syntax error: Didn't get what expected :(");
                exit(2);
            }
        case eof:
            if(defallowed)
                return true;
        default:
            fprintf(stderr, "Syntax error: Didn't get what expected :(");
            exit(2);
    }
    exit(0);
}

//bool p_expr(token_t * oldtoken, token_t * token)
//{
//
//}

bool p_const(token_t * token, token_type expect1, token_type expect2)
{
    token_t oldtoken = *token;
    *token = get_token(0);
    if(token->type != expect1 && token->type != expect2)
    {

        if(expr(&oldtoken, token, expect1, expect2, 0) == 0)
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
    if(token->type == ID_function)
    {
        *token = get_token(0);
        return p_fcall(token);
    }
    else //if(token->type == ID_variable)
    {
        token_t oldtoken = *token;
        *token = get_token(0);
        if(token->type != semicolon)
        {
            if(expr(&oldtoken, token, semicolon, semicolon, 0) == 0)
            {
                DPRINT(("%s was an expression.\n", oldtoken.value));
                //*token = get_token(0);
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
    //else if(token->type == ffloat || token->type == integer || token->type == string)
    //    return p_const(token);
    //else
    //{
    //    fprintf(stderr, "Syntax error: Expected a value, a function or variable, got %s\n", token->value);
    //    exit(2);
    //}
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
        fprintf(stderr, "Syntax error: Expected '(' after function call, got %s\n", token->value);
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
        return p_vals(token) && p_ncallargs(token);
    }
}

bool p_ncallargs(token_t *token)
{
    if(token->type == rbracket)
    {
        *token = get_token(0);
        return true;
    }
    else if(token->type == comma)
    {
        *token = get_token(0);
        return p_vals(token) && p_ncallargs(token);
    }
    else
    {
        fprintf(stderr, "Syntax error: NCALLARGS Error in handling function call arguments.\n");
        exit(2);
    }
}

bool p_vals(token_t *token)
{
    DPRINT(("Called p_vals with %s\n", token->value));
    if(token->type == ID_function)
    {
        *token = get_token(0);
        return p_fcall(token);
    }
    else
    {
        token_t oldtoken = *token;
        *token = get_token(0);
        if(token->type == comma || token->type == rbracket)
            {
                if(oldtoken.type == ID_variable || oldtoken.type == ffloat || oldtoken.type == integer || oldtoken.type == string)
                    return true;
                else
                {
                    fprintf(stderr, "Syntax error: Expected fID, vID or constant in function arg, got %s.\n", token->value);
                    exit(2);
                }
                //if(expr(&oldtoken, token, comma, rbracket, 0) == 0)
                //{
                //    DPRINT(("FIRST: %s was an expression in if statement\n", token->value));
                //    //*token = get_token(0);
                //    DPRINT(("%s\n", token->value));
                //    return true;
                //}
                //else
                //{
                //    fprintf(stderr, "Syntax error: Failed to handle expression.\n");
                //    exit(2);
                //}
            }
        else
        {
            fprintf(stderr, "Syntax error: Unexpected symbol in fcall: %s.\n", token->value);
            exit(2);
            // TOTO JE FUNEXP WTF
            //if(expr(&oldtoken, token, comma, rbracket, 0) == 0)
            //    {
            //        DPRINT(("SECOND: %s was an expression in if statement\n", token->value));
            //        //*token = get_token(0);
            //        DPRINT(("%s\n", token->value));
            //        return true;
            //    }
            //    else
            //    {
            //        fprintf(stderr, "Syntax error: Failed to handle expression.\n");
            //        exit(2);
            //    }
        }
    }
}

bool p_ifstat(token_t *token)
{
    *token = get_token(0);
    if(token->type == lbracket)
    {
        *token = get_token(0);
        if(token->type == ID_function)
        {
            DPRINT(("Calling fcall on %s\n", token->value));
            *token = get_token(0);
            if(p_fcall(token))
            {
                *token = get_token(0);
                DPRINT(("Calling ifbody with %s\n", token->value));
                return p_ifbody(token);
            }
            else
            {
                fprintf(stderr, "Syntax error: Function call mysteriously failed\n");
                exit(2);
            }
        }
        else
        {
            if(expr(NULL, token, rbracket, rbracket, 0) == 0)
            {
                DPRINT(("%s was an expression in if statement\n", token->value));
                *token = get_token(0);
                DPRINT(("Calling ifbody with %s\n", token->value));
                return p_ifbody(token);
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

bool p_ifbody(token_t *token)
{
    if(token->type == lsetbracket)
    {
        *token = get_token(0);
        return p_body(token, false) && p_elsebody(token);
    }
    else
    {
        fprintf(stderr, "Syntax error: Expected '{' after if argument, got '%s'\n", token->value);
        exit(2);
    }
}

bool p_elsebody(token_t *token)
{
    if(token->type == funelse)
    {
        *token = get_token(0);
        if(token->type == lsetbracket)
        {
            *token = get_token(0);
            return p_body(token, false);
        }
        else
        {
            fprintf(stderr, "Syntax error: Expected '{' after else, got '%s'\n", token->value);
            exit(2);
        }
    }
    else
    {
        fprintf(stderr, "Syntax error: 'else' statement required after if, got '%s'\n", token->value);
        exit(2);
    }
}

bool p_whilestat(token_t *token)
{
    *token = get_token(0);
    if(token->type == lbracket)
    {
        *token = get_token(0);
        if(token->type == ID_function)
        {
            DPRINT(("Calling fcall on %s\n", token->value));
            *token = get_token(0);
            if(p_fcall(token))
            {
                *token = get_token(0);
                DPRINT(("Calling whilebody with %s\n", token->value));
                return p_whilebody(token);
            }
            else
            {
                fprintf(stderr, "Syntax error: Function call mysteriously failed\n");
                exit(2);
            }
        }
        else
        {
            DPRINT(("TOKEN: %s\n", token->value));
            if(expr(NULL, token, rbracket, rbracket, 0) == 0)
            {
                DPRINT(("%s was an expression in if statement\n", token->value));
                *token = get_token(0);
                DPRINT(("Calling whilebody with %s\n", token->value));
                return p_whilebody(token);
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
        fprintf(stderr, "Syntax error: Expected '(' after 'while', got '%s'\n", token->value);
        exit(2);
    }
}

bool p_whilebody(token_t * token)
{
    if(token->type == lsetbracket)
    {
        *token = get_token(0);
        return p_body(token, false);
    }
    else
    {
        fprintf(stderr, "Syntax error: Expected '{' after while argument, got '%s'\n", token->value);
        exit(2);
    }
}

bool p_fundec(token_t * token)
{
    if(token->type == ID_function)
    {
        *token = get_token(0);
        return p_funargs(token) && p_funbody(token);
    }
    else
    {
        fprintf(stderr, "Syntax error: Expected function identificator after 'function' token, got '%s'\n", token->value);
        exit(2);
    }
    return true;
}

bool p_funargs(token_t * token)
{
    if(token->type == lbracket)
    {
        *token = get_token(0);
        if(p_fparams(token))
        {
            *token = get_token(0);
            return true;
        }
        return true;
    }
    else
    {
        fprintf(stderr, "Syntax error: Expected '(' after function identificator, got '%s'\n", token->value);
        exit(2);
    }
}

bool p_funbody(token_t * token)
{
    //if(token->type == colon)
    return true;
}

bool p_fparams(token_t * token)
{
    return true;
}
