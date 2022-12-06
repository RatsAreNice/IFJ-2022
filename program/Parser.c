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
#include "symdll.h"
#include "test.h"
#include "semantic.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "symtable.c"

bool parse();

bool p_start(token_t * token, ASSnode_t **astree);

bool p_prolog(token_t * token);
bool p_prolog_sub1(token_t * token);
bool p_prolog_sub2(token_t * token);

bool p_body(token_t * token, bool defallowed, ASSnode_t *astree, bool mainreturn, returnType_t rettyp);

bool p_fundec(token_t * token, ASSnode_t *astree);
bool p_fparams(token_t * token, ASSnode_t *astree, funData_t* newfunc);
bool p_nparam(token_t *token, ASSnode_t *astree, funData_t* newfunc);

//bool p_expr(token_t * token);

bool p_rettype(token_t * token);
bool p_type(token_t * token);

bool p_assigned(token_t *token, ASSnode_t *astree);
bool p_const(token_t * token, token_type expect1, token_type expect2);
bool p_fcall(token_t *token, ASSnode_t *astree);
bool p_callargs(token_t *token, ASSnode_t *astree, bst_node_t *symtableentry, int* paramcount);
bool p_ncallargs(token_t *token, ASSnode_t *astree, bst_node_t *symtableentry, int* paramcount);
bool p_vals(token_t *token, ASSnode_t* astree, bst_node_t *symtableentry, int* paramcount);

symDLList_t symtablelist;

typedef enum
{
    SKIP,
    NOSKIP
} scannermode;

void* safeMalloc(size_t size)
{
    void* new = malloc(size);
    if(new == NULL)
    {
        fprintf(stderr, "Malloc failure.\n");
        exit(99);
    }
    else
        return new;
}

void addDefaults(bst_node_t **symtable)
{
    //reads

    funData_t* new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symQString;
    new->ParamCount = 0;
    new->paramTypes = NULL;
    bst_insert(symtable, "reads", ID_function, new);
    
    //readi

    new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symQInt;
    new->ParamCount = 0;
    new->paramTypes = NULL;
    bst_insert(symtable, "readi", ID_function, new);

    //readf

    new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symQFloat;
    new->ParamCount = 0;
    new->paramTypes = NULL;
    bst_insert(symtable, "readf", ID_function, new);

    //write

    new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symVoid;
    new->ParamCount = -1;
    new->paramTypes = NULL;
    bst_insert(symtable, "write", ID_function, new);

    //floatval

    new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symFloat;
    new->ParamCount = 1;
    new->paramTypes = NULL;
    bst_insert(symtable, "floatval", ID_function, new);

    //intval

    new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symInt;
    new->ParamCount = 1;
    new->paramTypes = NULL;
    bst_insert(symtable, "intval", ID_function, new);

    //strval

    new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symString;
    new->ParamCount = 1;
    new->paramTypes = NULL;
    bst_insert(symtable, "strval", ID_function, new);

    //strlen

    new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symInt;
    new->ParamCount = 1;
    int* newparamtable = safeMalloc(sizeof(int)*new->ParamCount);
    newparamtable[0] = symString;
    new->paramTypes = newparamtable;
    bst_insert(symtable, "strlen", ID_function, new);

    //substring

    new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symQString;
    new->ParamCount = 3;
    newparamtable = safeMalloc(sizeof(int)*new->ParamCount);
    newparamtable[0] = symString;
    newparamtable[1] = symInt;
    newparamtable[2] = symInt;
    new->paramTypes = newparamtable;
    bst_insert(symtable, "substring", ID_function, new);

    //ord

    new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symInt;
    new->ParamCount = 1;
    newparamtable = safeMalloc(sizeof(int)*new->ParamCount);
    newparamtable[0] = symString;
    new->paramTypes = newparamtable;
    bst_insert(symtable, "ord", ID_function, new);

    //chr

    new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symString;
    new->ParamCount = 1;
    newparamtable = safeMalloc(sizeof(int)*new->ParamCount);
    newparamtable[0] = symInt;
    new->paramTypes = newparamtable;
    bst_insert(symtable, "chr", ID_function, new);

}

bool parse()
{
    // Init main symtable a pridat default funkcie.
    bst_node_t* mainsymtable;

    bst_init(&mainsymtable);
    addDefaults(&mainsymtable);
    //symDLList_t symtablelist;
    symDLL_Init(&symtablelist);
    symDLL_InsertLast(&symtablelist, mainsymtable, NULL);
    symDLL_First(&symtablelist);

    token_t token = get_token(NOSKIP);
    ASSnode_t *root;
    ASSinit(&root);
    //*root = makeTree(RYAN_GOSLING, NULL, NULL);
    if ( p_start(&token, &root) == false )
    {
        fprintf(stderr, "Unknown syntax error.\n");
        exit(2);
    }
    PRINTTREEFAST(root);
    return true;
}

bool p_start(token_t * token, ASSnode_t **astree)
{
    (*astree) = makeTree(RYAN_GOSLING, NULL, NULL);
    return p_prolog(token) && p_body(token, true, *astree, true, symInt);
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
        *token = get_token(SKIP);
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
        *token = get_token(SKIP);
        return true;
    }
    else
    {
        fprintf(stderr, "Syntax error: Malformed prolog at beginning of file, got: %s\n", token->value);
        exit(2);
    }
    return false;
}

bool p_body(token_t * token, bool defallowed, ASSnode_t *astree, bool mainreturn, returnType_t rettyp)
{
    token_t oldtoken;
    DPRINT(("Got to body with token %s of type %d\n", token->value, token->type));
    switch(token->type)
    {
        case funreturn:
            *token = get_token(SKIP);
            astree->left = makeTree(RETURN, NULL, NULL);
            if(token->type == semicolon) // RETURN NOTHING, ONLY USABLE FOR VOID -> "RETURN;" 
            {
                // Ak nie je v "main" tele programu, a return type nematchuje, chyba
                if(!mainreturn && rettyp != symVoid)
                {
                    fprintf(stderr, "Semantic error: Attempted to return nothing in a non-void function.");
                    exit(4);
                }
                *token = get_token(SKIP);
                astree->right = makeTree(RYAN_GOSLING, NULL, NULL);
                return p_body(token, defallowed, astree->right, mainreturn, rettyp);
            }
            else if(token->type == ID_function) // RETURN CALLS ANOTHER FUNCTION -> "RETURN readi();" 
            {
                bst_node_t* symtableptr = isDefined(token, symDLL_GetFirst(&symtablelist)); // Pozrie, ci funkcie je deklarovana v symtabli a vrati ukazatel na nu.
                if(!mainreturn && rettyp != symtableptr->funData->returnType) // Ak sme mimo hlavneho tela programu a navratovy typ sa nezhoduje s ocakavanym -> error.
                {
                    fprintf(stderr, "Semantic error: Wrong return type.");
                    exit(4);
                }
                astree->left->left = makeTree(FUNCTIONCALL, NULL, makeLeaf(token));
                if(p_fcall(token, astree->left->left))
                {
                    if(token->type == semicolon)
                    {
                        *token = get_token(SKIP);
                        astree->right = makeTree(RYAN_GOSLING, NULL, NULL);
                        return p_body(token, defallowed, astree->right, mainreturn, rettyp);
                    }
                    else
                    {
                        fprintf(stderr, "Syntax Error: Expected ';' after return.\n");
                        exit(2);
                    }
                    
                }
                else
                {
                    fprintf(stderr, "Syntax error: Function call failed in body.");
                    exit(2);
                }
            }
            else // RETURN <EXPRESSION>;
            {
                ASSnode_t *exprptr = NULL;
                exprptr = expr(NULL, token, semicolon, semicolon, 0);
                if(exprptr != NULL)
                {   
                    astree->left->left = exprptr;
                    DPRINT(("%s\n", token->value));
                    *token = get_token(SKIP);
                    //DPRINT(("%s\n", token->value));
                    astree->right = makeTree(RYAN_GOSLING, NULL, NULL);
                    return p_body(token, defallowed,astree->right, mainreturn, rettyp);
                }
                else
                {
                    fprintf(stderr, "Syntax error: Failed to handle expression.\n");
                    exit(2);
                }
            }
            return false;

        case ID_function:
            DPRINT(("ID FUNCTION %s of type %d\n", token->value, token->type));
            astree->left = makeTree(FUNCTIONCALL, NULL, makeLeaf(token));
            if(p_fcall(token, astree->left))
            {
                if(token->type == semicolon)
                {
                    *token = get_token(SKIP);
                    astree->right = makeTree(RYAN_GOSLING, NULL, NULL);
                    return p_body(token,defallowed, astree->right, mainreturn, rettyp);
                }
                else
                {
                    fprintf(stderr, "Syntax Error: Expected ';' after function.\n");
                    exit(2);
                }
            }
            else
            {
                fprintf(stderr, "Syntax error: Function call failed in body.");
                exit(2);
            }
        case lbracket:
        case ffloat:
        case integer:
        case string:
            if(p_const(token, semicolon, semicolon))
            {
                *token = get_token(SKIP);
                astree->right = makeTree(RYAN_GOSLING, NULL, NULL);
                return true && p_body(token, defallowed, astree->right, mainreturn, rettyp);
            }
            else
            {
                fprintf(stderr, "Syntax error: Didn't get what expected :(");
                exit(2);
            }
        case ID_variable:
            oldtoken = *token;
            *token = get_token(SKIP);
            if(token->type != assign)
            {
                if(isDefined(&oldtoken, symtablelist.activeElement->symtable) == NULL)
                {
                    fprintf(stderr,"Semantic analysis error: Variable %s is not defined.\n", oldtoken.value);
                    exit(5);
                }
                ASSnode_t* exprptr = NULL;
                exprptr = expr(&oldtoken, token, semicolon, semicolon, 0);
                if(exprptr != NULL)
                {   
                    *token = get_token(SKIP);
                    astree->right = makeTree(RYAN_GOSLING, NULL, NULL);
                    return true && p_body(token, defallowed, astree->right, mainreturn, rettyp);
                }
                else
                {
                    fprintf(stderr, "Syntax error: Unknown token.\n");
                    exit(2);
                }
            }
            else
            {
                bst_insert(&(symtablelist.activeElement->symtable),oldtoken.value,token->type, NULL);
                astree->left = makeTree(ASSIGN, NULL, makeLeaf(&oldtoken));
                *token = get_token(SKIP);
                if(p_assigned(token, astree->left))
                {
                    *token = get_token(SKIP);
                    astree->right = makeTree(RYAN_GOSLING, NULL, NULL);
                    return true && p_body(token, defallowed, astree->right, mainreturn, rettyp);
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
                astree->left = makeTree(FDEC, NULL, makeTree(RYAN_GOSLING, NULL, NULL));
                *token = get_token(SKIP);
                if(p_fundec(token, astree->left))
                {
                    astree->right = makeTree(RYAN_GOSLING, NULL, NULL);
                    return p_body(token, defallowed, astree->right, mainreturn, rettyp);
                }
                else
                {
                    fprintf(stderr, "Syntax error: Unknown error in function declaration.\n");
                    exit(2);
                }
            }
        case epilog:
            if(!defallowed)
            {
                fprintf(stderr, "Syntax error: Can't use epilog in the inside of if/while/function.\n");
                exit(2);
            }
            else
            {
                *token = get_token(NOSKIP);
                if(token->type != eof)
                {
                    fprintf(stderr, "Syntax error: Tokens found after epilog\n");
                    exit(2);
                }
                else
                    return true;
            }
        case funif:
            astree->left = makeTree(IF, NULL, NULL);
            if(p_ifstat(token, astree->left, mainreturn, rettyp))
            {
                astree->right = makeTree(RYAN_GOSLING, NULL, NULL);
                return p_body(token, defallowed, astree->right, mainreturn, rettyp);
            }
            else
            {
                fprintf(stderr, "Syntax error: Unknown error in if statement\n");
                exit(2);
            }
            
        case funwhile:
            astree->left = makeTree(WHILE, NULL, NULL);
            if(p_whilestat(token, astree->left, mainreturn, rettyp))
            {
                astree->right = makeTree(RYAN_GOSLING, NULL, NULL);
                return p_body(token, defallowed, astree->right, mainreturn, rettyp);
            }
            else
            {
                fprintf(stderr, "Syntax error: Unknown error in while statement\n");
                exit(2);
            }
        case rsetbracket:
            if(!defallowed)
            {
                //if(!mainreturn)
                //{
                //    symDLL_Previous(&symtablelist);
                //    symDLL_DeleteLast(&symtablelist);
                //}
                *token = get_token(SKIP);
                return true;
            }
            else
            {
                fprintf(stderr, "Syntax error: Unexpected token '}'.\n");
                exit(2);
            }
        case eof:
            if(defallowed)
                return true;
        default:
            fprintf(stderr, "Syntax error: Didn't get what expected :(\n");
            exit(2);
    }
    exit(0);
}

bool p_const(token_t * token, token_type expect1, token_type expect2)
{
    token_t oldtoken = *token;
    *token = get_token(SKIP);
    if(token->type != expect1 && token->type != expect2)
    {
        ASSnode_t* exprptr = NULL;
        exprptr = expr(&oldtoken, token, expect1, expect2, 0);
        if(exprptr != NULL)
        //if(expr(&oldtoken, token, expect1, expect2, 0) == 0)
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

bool p_assigned(token_t *token, ASSnode_t *astree)
{
    if(token->type == ID_function)
    {
        astree->left = makeTree(FUNCTIONCALL, NULL, makeLeaf(token));
        return p_fcall(token, astree->left);
    }
    else
    {
        token_t oldtoken = *token;
        *token = get_token(SKIP);
        ASSnode_t* exprptr = NULL;
        exprptr = expr(&oldtoken, token, semicolon, semicolon, 0);
        if(exprptr != NULL)
        {
            DPRINT(("%s was an expression.\n", oldtoken.value));
            astree->left = exprptr;
            return true;
        }
        else
        {
            fprintf(stderr, "Syntax error: Failed to handle expression.\n");
            exit(2);
        }
    }
}

bool p_fcall(token_t *token, ASSnode_t *astree)
{
    //if(symtablelist.activeElement->fundata != NULL)
    //{
    //    //bool found = false;
    //    //for(int i = 0; i < symtablelist.activeElement->fundata->ParamCount; i++)
    //    //{
    //    //    if(strcmp(symtablelist.activeElement->fundata->dependencies[i], token->value) == 0)
    //    //    {
    //    //        found = true;
    //    //        break;
    //    //    }
    //    //}
    //    //if(!found)
    //    //{
    //    //    symtablelist.activeElement->fundata->ParamCount += 1;
    //    //    symtablelist.activeElement->fundata->dependencies = realloc(symtablelist.activeElement->fundata->dependencies, sizeof(char*))
    //    //}
    //}
    bst_node_t* symtableptr = isDefined(token, symDLL_GetFirst(&symtablelist));
    *token = get_token(SKIP);
    if(token->type == lbracket)
    {
        int paramcount = 0;
        *token = get_token(SKIP);
        if(p_callargs(token, astree, symtableptr, &paramcount))
        {
            DPRINT(("RETURNED WITH: %s\n",token->value));
            return true;
        }
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

bool p_callargs(token_t *token, ASSnode_t *astree, bst_node_t *symtableentry, int* paramcount)
{
    if(token->type == rbracket)
    {
        if(symtableentry->funData->ParamCount != *paramcount)
        {
            fprintf(stderr, "Semantic error: Argument count mismatch for %s, expected: %d, got %d.\n", symtableentry->key, symtableentry->funData->ParamCount, *paramcount);
            exit(4);
        }
        *token = get_token(SKIP);
        return true;
    }
    else
    {
        *paramcount += 1;
        astree->left = makeTree(ARGS, NULL, NULL);
        return p_vals(token, astree->left, symtableentry, paramcount) && p_ncallargs(token, astree->left, symtableentry, paramcount);
    }
}

bool p_ncallargs(token_t *token, ASSnode_t *astree, bst_node_t *symtableentry, int* paramcount)
{
    if(token->type == rbracket)
    {
        if(symtableentry->funData->ParamCount != -1 && symtableentry->funData->ParamCount != *paramcount)
        {
            fprintf(stderr, "ASSSSSSSemantic error: Argument count mismatch for %s, expected: %d, got %d.\n", symtableentry->key, symtableentry->funData->ParamCount, *paramcount);
            exit(4);
        }
        *token = get_token(SKIP);
        return true;
    }
    else if(token->type == comma)
    {
        astree->right = makeTree(ARGS, NULL, NULL);
        *token = get_token(SKIP);
        *paramcount += 1;
        return p_vals(token, astree->right, symtableentry, paramcount) && p_ncallargs(token, astree->right, symtableentry, paramcount);
    }
    else
    {
        fprintf(stderr, "Syntax error: NCALLARGS Error in handling function call arguments.\n");
        exit(2);
    }
}

bool p_vals(token_t *token, ASSnode_t* astree, bst_node_t *symtableentry, int* paramcount)
{
    DPRINT(("Called p_vals with %s\n", token->value));
    if(token->type == ID_function)
    {
        bst_node_t* symtableptr = isDefined(token, symDLL_GetFirst(&symtablelist));
        if(symtableentry->funData->ParamCount != -1)
        {
            if(symtableentry->funData->ParamCount < *paramcount)
            {
                fprintf(stderr, "Semantic error: Argument count mismatch for %s, expected: %d, got %d.\n", symtableentry->key, symtableentry->funData->ParamCount, *paramcount);
                exit(4);
            }
            if (symtableentry->funData->paramTypes[(*paramcount)-1] != symtableptr->funData->returnType)
            {
                fprintf(stderr, "Semantic error: Argument type %d number %d doesn't match return type %d of function %s\n",
                        symtableentry->funData->paramTypes[(*paramcount)-1], *paramcount, symtableptr->funData->returnType, symtableptr->key);
                exit(4);
            }
        }
        astree->left = makeTree(FUNCTIONCALL, NULL, makeLeaf(token));
        return p_fcall(token, astree->left);
    }
    else
    {
        token_t oldtoken = *token;
        *token = get_token(SKIP);
        if(token->type == comma || token->type == rbracket || token->type == semicolon)
            {
                if(oldtoken.type == ID_variable || oldtoken.type == ffloat || oldtoken.type == integer || oldtoken.type == string)
                {
                    if(symtableentry->funData->ParamCount != -1)
                    {   
                        if(symtableentry->funData->ParamCount < *paramcount)
                        {
                            fprintf(stderr, "Semantic error: Argument count mismatch for %s, expected: %d, got %d.\n", symtableentry->key, symtableentry->funData->ParamCount, *paramcount);
                            exit(4);
                        }
                        else if(oldtoken.type != ID_variable && oldtoken.type != symtableentry->funData->paramTypes[(*paramcount)-1])
                        {
                            fprintf(stderr, "Semantic error: Argument type %d number %d doesn't match type %d of literal %s\n",
                            symtableentry->funData->paramTypes[(*paramcount)-1], *paramcount, oldtoken.type, oldtoken.value);
                            exit(4);
                        }
                    }
                    astree->left = makeLeaf(&oldtoken);
                    return true;
                }
                else
                {
                    fprintf(stderr, "Syntax error: Expected fID, vID or constant in function arg, got %s.\n", token->value);
                    exit(2);
                }
            }
        else
        {
            ASSnode_t *exprptr = NULL;
            exprptr = expr(&oldtoken, token, comma, rbracket, 0);
            if(exprptr != NULL)
            {
                DPRINT(("SECOND: %s was an expression in if statement\n", oldtoken.value));
                astree->left = exprptr;
                DPRINT(("%s\n", token->value));
                return true;
            }
            else
            {
                fprintf(stderr, "Syntax error: Failed to handle expression.\n");
                exit(2);
            }
        }
    }
}

bool p_ifstat(token_t * token, ASSnode_t *astree, bool mainreturn, returnType_t rettyp)
{
    *token = get_token(SKIP);
    if(token->type == lbracket)
    {
        *token = get_token(SKIP);
        if(token->type == ID_function)
        {
            DPRINT(("Calling fcall on %s\n", token->value));
            astree->left = makeTree(FUNCTIONCALL, NULL, makeLeaf(token));
            if(p_fcall(token, astree->left))
            {
                *token = get_token(SKIP);
                astree->right = makeTree(THEN, NULL, NULL);
                DPRINT(("Calling ifbody with %s\n", token->value));
                return p_ifbody(token, astree->right, mainreturn, rettyp);
            }
            else
            {
                fprintf(stderr, "Syntax error: Function call mysteriously failed\n");
                exit(2);
            }
        }
        else
        {
            ASSnode_t *exprptr = NULL;
            exprptr = expr(NULL, token, rbracket, rbracket, 0);
            if(exprptr != NULL)
            {
                astree->left = exprptr;
                astree->right = makeTree(THEN, NULL, NULL);
                *token = get_token(SKIP);
                DPRINT(("Calling ifbody with %s\n", token->value));
                return p_ifbody(token, astree->right, mainreturn, rettyp);
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

bool p_ifbody(token_t *token, ASSnode_t *astree, bool mainreturn, returnType_t rettyp)
{
    if(token->type == lsetbracket)
    {
        *token = get_token(SKIP);
        astree->left = makeTree(RYAN_GOSLING, NULL, NULL);
        astree->right = makeTree(RYAN_GOSLING, NULL, NULL);
        return p_body(token, false, astree->left, mainreturn, rettyp) && p_elsebody(token, astree->right, mainreturn, rettyp);
    }
    else
    {
        fprintf(stderr, "Syntax error: Expected '{' after if argument, got '%s'\n", token->value);
        exit(2);
    }
}

bool p_elsebody(token_t *token, ASSnode_t *astree, bool mainreturn, returnType_t rettyp)
{
    if(token->type == funelse)
    {
        *token = get_token(SKIP);
        if(token->type == lsetbracket)
        {
            *token = get_token(SKIP);
            return p_body(token, false, astree, mainreturn, rettyp);
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

bool p_whilestat(token_t * token, ASSnode_t *astree, bool mainreturn, returnType_t rettyp)
{
    *token = get_token(SKIP);
    if(token->type == lbracket)
    {
        *token = get_token(SKIP);
        if(token->type == ID_function)
        {
            DPRINT(("Calling fcall on %s\n", token->value));
            astree->left = makeTree(FUNCTIONCALL, NULL, makeLeaf(token));
            if(p_fcall(token, astree->left))
            {
                *token = get_token(SKIP);
                astree->right = makeTree(RYAN_GOSLING, NULL, NULL);
                DPRINT(("Calling whilebody with %s\n", token->value));
                return p_whilebody(token, astree->right, mainreturn, rettyp);
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
            ASSnode_t *exprptr = NULL;
            exprptr = expr(NULL, token, rbracket, rbracket, 0);
            if(exprptr != NULL)
            {
                astree->left = exprptr;
                DPRINT(("%s was an expression in if statement\n", token->value));
                *token = get_token(SKIP);
                astree->right = makeTree(RYAN_GOSLING, NULL, NULL);
                DPRINT(("Calling whilebody with %s\n", token->value));
                return p_whilebody(token, astree->right,mainreturn, rettyp);
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

bool p_whilebody(token_t * token, ASSnode_t *astree, bool mainreturn, returnType_t rettyp)
{
    if(token->type == lsetbracket)
    {
        *token = get_token(SKIP);
        return p_body(token, false, astree, mainreturn, rettyp);
    }
    else
    {
        fprintf(stderr, "Syntax error: Expected '{' after while argument, got '%s'\n", token->value);
        exit(2);
    }
}

bool p_fundec(token_t * token, ASSnode_t *astree)
{
    if(token->type == ID_function)
    {
        funData_t* newfunc = safeMalloc(sizeof(struct funData));
        newfunc->defined = true;
        newfunc->ParamCount = 0;
        newfunc->depCount = 0;
        newfunc->dependencies = NULL;
        newfunc->paramTypes = NULL;

        astree->left = makeTree(FDEC_DATA, makeTree(FDEC_DATA_INF, makeLeaf(token), NULL), NULL);

        *token = get_token(SKIP);

        bst_node_t *newscope;
        bst_init(&newscope);
        symDLL_InsertLast(&symtablelist, newscope, newfunc);
        symDLL_Next(&symtablelist);

        return p_funargs(token, astree->left, newfunc) && p_funbody(token, astree, newfunc);
    }
    else
    {
        fprintf(stderr, "Syntax error: Expected function identificator after 'function' token, got '%s'\n", token->value);
        exit(2);
    }
    return true;
}

bool p_funargs(token_t * token, ASSnode_t *astree, funData_t* newfunc)
{
    if(token->type == lbracket)
    {
        *token = get_token(SKIP);
        astree->right = makeTree(FDEC_PARAMS, NULL, NULL);
        return p_fparams(token, astree->right, newfunc);
    }
    else
    {
        fprintf(stderr, "Syntax error: Expected '(' after function identificator, got '%s'\n", token->value);
        exit(2);
    }
}

bool p_funbody(token_t * token, ASSnode_t *astree, funData_t* newfunc)
{
    if(token->type == colon)
    {
        *token = get_token(SKIP);
        if(token->type == funvoid || token->type == type)
        {
            astree->left->left->right = makeLeaf(token);
            newfunc->returnType = translate(token->value);
            bst_node_t* topsymtable = symDLL_GetFirst(&symtablelist);
            bst_insert(&topsymtable, astree->left->left->left->Patrick_Bateman->value,ID_function,newfunc);
            *token = get_token(SKIP);
        }
        else
        {
            fprintf(stderr, "Syntax error: Expected return type of function, got '%s'\n", token->value);
            exit(2);
        }
    }
    else
    {
        fprintf(stderr, "Syntax error: Expected ':' after function declaration arguments, got '%s'\n", token->value);
        exit(2);
    }
    if(token->type == lsetbracket)
    {
        //returnType_t rettype;
        *token = get_token(SKIP);
        // Preložiť typ na returntyp_t
        //if(strcmp(astree->left->left->right->Patrick_Bateman->value, "int") == 0)
        //    rettype = symInt;
        //else if(strcmp(astree->left->left->right->Patrick_Bateman->value, "float") == 0)
        //    rettype = symFloat;
        //else if(strcmp(astree->left->left->right->Patrick_Bateman->value, "string") == 0)
        //    rettype = symString;
        //else if(strcmp(astree->left->left->right->Patrick_Bateman->value, "?float") == 0)
        //    rettype = symQFloat;
        //else if(strcmp(astree->left->left->right->Patrick_Bateman->value, "?int") == 0)
        //    rettype = symQInt;
        //else if(strcmp(astree->left->left->right->Patrick_Bateman->value, "?string") == 0)
        //    rettype = symQString;
        //else
        //    rettype = astree->left->left->right->Patrick_Bateman->type;
        if(p_body(token, false, astree->right, false, translate(astree->left->left->right->Patrick_Bateman->value)))
        {
            symDLL_Previous(&symtablelist);
            symDLL_DeleteLast(&symtablelist);
            return true;
        }
        else
        {
            fprintf(stderr, "Unknown error in function declaration\n");
            exit(2);
        }
        //return p_body(token, false, astree->right, false, translate(astree->left->left->right->Patrick_Bateman->value));
    }
    else
    {
        fprintf(stderr, "Syntax error: Expected '{' after function declaration, got '%s'\n", token->value);
        exit(2);
    }
    return true;
}

bool p_fparams(token_t * token, ASSnode_t *astree, funData_t* newfunc)
{
    if(token->type == rbracket)
    {
        *token = get_token(SKIP);
        return true;
    }
    else if(token->type == type)
    {
        newfunc->ParamCount += 1;
        newfunc->paramTypes = realloc(newfunc->paramTypes, (newfunc->ParamCount)*sizeof(int));
        newfunc->paramTypes[newfunc->ParamCount-1] = translate(token->value);
        astree->left = makeTree(FDEC_NPARAM, makeLeaf(token), NULL);
        DPRINT(("%s TYPE IS TYPE \n", token->value));
        *token = get_token(SKIP);
        if(token->type == ID_variable)
        {
            astree->left->right = makeLeaf(token);
            DPRINT(("%s TYPE IS VAR \n", token->value));
            *token = get_token(SKIP);
            return p_nparam(token, astree, newfunc);
        }
        else
        {
            fprintf(stderr, "Syntax error: Expected vID after type in function declaration parameters, got '%s'\n", token->value);
            exit(2);
        }
    }
    else
    {
    fprintf(stderr, "Syntax error: Expected '(' after function id in function declaration, got '%s'\n", token->value);
    exit(2);
    }
}

bool p_nparam(token_t *token, ASSnode_t *astree, funData_t* newfunc)
{
    if(token->type == rbracket)
    {
        *token = get_token(SKIP);
        return true;
    }
    else if(token->type == comma)
    {
        *token = get_token(SKIP);
        astree->right = makeTree(FDEC_PARAMS, NULL, NULL);
        return p_fparams(token, astree->right, newfunc);
    }
    else
    {
        fprintf(stderr, "Syntax error: NCALLARGS Error in handling function call arguments.\n");
        exit(2);
    }
}