// Nazov projektu: IFJ22
// Autor: Richard Blažo (xblazo00)
// Parser

//#define DEBUG
#ifdef DEBUG
# define DPRINT(x) fprintf x
#else
# define DPRINT(x) do {} while (0)
#endif

// Makra pre kvalitu zivota
#define fdepcount symtablelist.activeElement->fundata->depCount
#define fdeplist symtablelist.activeElement->fundata->dependencies

#include "scanner.h"
#include "bottom_up.h"
#include "symtable.h"
#include "Parser.h"
#include "symdll.h"
#include "semantic.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// Globalna premenna na ulozenie obojstranne viazaneho zoznamu na ukladanie tabuliek.
symDLList_t symtablelist;

/// @brief Funkcia ktora vyriesi problem pri mallocu ak nastane.
/// @param OP size Pocet bajtov pamate ktore treba alokovat.
/// @return Vracia odkaz na alokovanu pamat.
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

/// @brief Funkcia na vlozenie vstavanych funkcii jazyka IFJ22.
/// @param symtable Tabulka symbolov do ktorej budu funkcie vlozene.
void addDefaults(bst_node_t **symtable)
{
    //reads

    funData_t* new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symQString;
    new->ParamCount = 0;
    new->paramTypes = NULL;
    new->depCount = 0;
    new->dependencies = NULL;
    bst_insert(symtable, "reads", ID_function, new);
    
    //readi

    new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symQInt;
    new->ParamCount = 0;
    new->paramTypes = NULL;
    new->depCount = 0;
    new->dependencies = NULL;
    bst_insert(symtable, "readi", ID_function, new);

    //readf

    new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symQFloat;
    new->ParamCount = 0;
    new->paramTypes = NULL;
    new->depCount = 0;
    new->dependencies = NULL;
    bst_insert(symtable, "readf", ID_function, new);

    //write

    new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symVoid;
    new->ParamCount = -1;
    new->paramTypes = NULL;
    new->depCount = 0;
    new->dependencies = NULL;
    bst_insert(symtable, "write", ID_function, new);

    //floatval

    new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symFloat;
    new->ParamCount = 1;
    new->paramTypes = NULL;
    new->depCount = 0;
    new->dependencies = NULL;
    bst_insert(symtable, "floatval", ID_function, new);

    //intval

    new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symInt;
    new->ParamCount = 1;
    new->paramTypes = NULL;
    new->depCount = 0;
    new->dependencies = NULL;
    bst_insert(symtable, "intval", ID_function, new);

    //strval

    new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symString;
    new->ParamCount = 1;
    new->paramTypes = NULL;
    new->depCount = 0;
    new->dependencies = NULL;
    bst_insert(symtable, "strval", ID_function, new);

    //strlen

    new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symInt;
    new->ParamCount = 1;
    int* newparamtable = safeMalloc(sizeof(int)*new->ParamCount);
    newparamtable[0] = symString;
    new->paramTypes = newparamtable;
    new->depCount = 0;
    new->dependencies = NULL;
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
    new->depCount = 0;
    new->dependencies = NULL;
    bst_insert(symtable, "substring", ID_function, new);

    //ord

    new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symInt;
    new->ParamCount = 1;
    newparamtable = safeMalloc(sizeof(int)*new->ParamCount);
    newparamtable[0] = symString;
    new->paramTypes = newparamtable;
    new->depCount = 0;
    new->dependencies = NULL;
    bst_insert(symtable, "ord", ID_function, new);

    //chr

    new = safeMalloc(sizeof(struct funData));
    new->defined = true;
    new->returnType = symString;
    new->ParamCount = 1;
    newparamtable = safeMalloc(sizeof(int)*new->ParamCount);
    newparamtable[0] = symInt;
    new->paramTypes = newparamtable;
    new->depCount = 0;
    new->dependencies = NULL;
    bst_insert(symtable, "chr", ID_function, new);

}

/// @brief Funkcia, kde sa pripravi obojstranne viazany zoznam
///        a tabulky symbolov a zacne prechadzat program.
void parse()
{
    bst_node_t* mainsymtable;

    bst_init(&mainsymtable);
    addDefaults(&mainsymtable);

    symDLL_Init(&symtablelist);
    symDLL_InsertLast(&symtablelist, mainsymtable, NULL);
    symDLL_First(&symtablelist);

    token_t token = get_token(NOSKIP);
    ASSnode_t *root;
    ASSinit(&root);

    if ( p_start(&token, &root) == false )
    {
        fprintf(stderr, "Unknown syntax error.\n");
        exit(2);
    }
    print_code(&root);
}

/// @brief Prvy krok rekurzivneho zostupu, skontroluje prolog
///        a pokracuje v hlavnom tele programu.
/// @param token Ukazatel na spracovavany token.
/// @param astree Ukazatel na koren syntaktickeho stromu.
/// @return Celkové vyhodnotenie rekurzivneho zostup.
bool p_start(token_t * token, ASSnode_t **astree)
{
    (*astree) = makeTree(RYAN_GOSLING, NULL, NULL);
    return p_prolog(token) && p_body(token, true, *astree, true, symInt);
}

/// @brief Kontrola prologu.
/// @param token Ukazatel na spracovavany token.
/// @return True ak obidva tokeny tvoria zaciatocny prolog
///         inak ukonci program s chybou.
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

/// @brief Kontrola prvej casti prologu.
/// @param token Ukazatel na spracovavany token.
/// @return True ak je token prva cast prologu,
///         inak ukonci program s chybou.
bool p_prolog_sub1(token_t * token)
{
    if(token->type == prolog1)
    {
        
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

/// @brief Kontrola druhej casti prologu.
/// @param token Ukazatel na spracovavany token.
/// @return True ak je token prva cast prologu,
///         inak ukonci program s chybou.
bool p_prolog_sub2(token_t * token)
{
    if(token->type == prolog2)
    {
        
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

/// @brief Funkcia na analyzu hlavneho tela, tela funkcie,
///        a tela if, else a while.
/// @param token Ukazatel na spracovavany token.
/// @param defallowed Povolenie definicie funkcii, indikuje ze sa analyzator nachadza v if, while alebo funkcii.
/// @param astree Ukazatel na abstraktny syntakticky strom.
/// @param mainreturn Premenna, ktora oznacuje ci sa analyzator nachadza v hlavnom tele programu (nie vo funkcii).
/// @param rettyp Ocakavany navratovy typ.
/// @return True po vyhodnoteni vsetkych rekurzivnych zostupov,
///         v pripade neznameho/neocakavaneho tokenu ukonci program.
bool p_body(token_t * token, bool defallowed, ASSnode_t *astree, bool mainreturn, returnType_t rettyp)
{
    token_t oldtoken;
    
    switch(token->type)
    {
        // Najdeny token je 'return':
        case funreturn:
            *token = get_token(SKIP);
            astree->left = makeTree(RETURN, NULL, NULL);
            if(token->type == semicolon) // Return bez argumentu -> moze byt pouzity len vo void funkciach
            {
                // Ak nie je v "main" tele programu, a return type nematchuje, chyba
                if(!mainreturn && rettyp != symVoid)
                {
                    fprintf(stderr, "Semantic error: Attempted to return nothing in a non-void function.");
                    exit(6);
                }
                *token = get_token(SKIP);
                astree->right = makeTree(RYAN_GOSLING, NULL, NULL);
                return p_body(token, defallowed, astree->right, mainreturn, rettyp);
            }
            else if(token->type == ID_function) // Return s funkciou ako argumentom.
            {
                bst_node_t* symtableptr = checkDefined(token, symDLL_GetFirst(&symtablelist)); // Pozrie, ci funkcie je deklarovana v symtabli a vrati ukazatel na nu.
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
            else // Return s inym tokenom ako argumentom -> expression.
            {
                ASSnode_t *exprptr = NULL;
                exprptr = expr(NULL, token, semicolon, semicolon, 0);
                if(exprptr != NULL)
                {   
                    astree->left->left = exprptr;
                    
                    *token = get_token(SKIP);
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
        // Najdeny token je ID funkcie.
        case ID_function:
            
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
        // Najdeny token je '(', literal float, string alebo integer
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
        // Najdeny token je ID premennej.
        case ID_variable:
            // Moze byt assign alebo expression -> musime sa pozriet na dalsi token.
            oldtoken = *token;
            *token = get_token(SKIP);
            if(token->type != assign) // Ak dalsi token nie je '=' -> spracovavame výraz
            {
                if(checkDefined(&oldtoken, symtablelist.activeElement->symtable) == NULL)
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
            else // Dalsi token je '=', spracovavame priradenie.
            {
                bst_insert(&(symtablelist.activeElement->symtable),oldtoken.value,oldtoken.type, NULL);
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
        // Najdeny token je 'function'
        case function:
            if(!defallowed) // Ak sa nachadzame vo if/while/deklaracii funkcie, vypise chybu a ukonci program.
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
        // Najdeny token je '?>'
        case epilog:
            if(!defallowed) // Ak sa nachadzame v if/while/deklaracii funkcie, vypie chybu a ukonci program.
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
        // Najdeny token je 'if'
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
        // Najdeny token je 'while'
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
        // Najdeny token je '}'
        case rsetbracket:
            if(!defallowed) // Ak sme v if/while/definicii funkcie, ukonci spracovavanie tela a vrati true. 
            {
                *token = get_token(SKIP);
                return true;
            }
            else
            {
                fprintf(stderr, "Syntax error: Unexpected token '}'.\n");
                exit(2);
            }
        // Najdeny token je <EOF>
        case eof:
            if(defallowed) // Ak nie sme v if/while/definicii funkcie, ukonci spracovavanie hlavného tela a vrati true.
                return true;
        // Ak je token <EOF> a sme v if/while/definicii funkcie alebo sme dostali neocakavany token.
        default: 
            fprintf(stderr, "Syntax error: Didn't get what expected :(\n");
            exit(2);
    }
    exit(0);
}

/// @brief Funkcia spracuje konstantu/vyraz. 
/// @param token Ukazatel na spracovavany token.
/// @param expect1 Prvy typ tokenu ktory oznacuje koniec konstanty/vyrazu.
/// @param expect2 Druhy typ tokenu ktory oznacuje koniec konstanty/vyrazu.
/// @return True ak sa jedna o platnu konstantu/vyraz, inak ukonci program s chybou.
bool p_const(token_t * token, token_type expect1, token_type expect2)
{
    token_t oldtoken = *token;
    *token = get_token(SKIP);
    if(token->type != expect1 && token->type != expect2)
    {
        ASSnode_t* exprptr = NULL;
        exprptr = expr(&oldtoken, token, expect1, expect2, 0);
        if(exprptr != NULL)
        {
            
            return true;
        }
        else
        {
            fprintf(stderr, "Syntax error: Failed to handle expression.\n");
            exit(2);
        }
    }
    else // Ak je dalsi token ukoncujuci typ tokenu.
    {
        
        return true;
    }
}

/// @brief Funkcia spracuje priradenie
/// @param token Ukazatel na spracovavany token.
/// @param astree Ukazatel na abstraktny syntakticky strom.
/// @return True, v pripade ze je priradena funkcia, konstanta alebo výraz
///         inak ukonci program s chybou.
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

/// @brief Funkcia spracuje volanie funkcie.
/// @param token Ukazatel na spracovavany token.
/// @param astree Ukazatel na abstraktny syntakticky strom.
/// @return Vracia true pri spravnej definicii funkcie, inak skonci program s chybou.
bool p_fcall(token_t *token, ASSnode_t *astree)
{
    // Pomocne premenne na semanticku analyzu
    bst_node_t* symtableptr = NULL;
    int fundataindex = -1;
    // symtablelist.activeElement->fundata != NULL -> znamena ze analyzator sa nachadza v definicii funkcie.
    // Ak nie sme v definicie funkcie alebo uz je definovana.
    if(symtablelist.activeElement->fundata != NULL && !isDefined(token, symDLL_GetFirst(&symtablelist))) 
    {
        // Nachadzame sa vo funkcii -> nemusime kontrolovat ci volana funkcia je definovana -> pridame
        // do listu zavislosti, skontroluje sa pri volani funkcie.
        for(int i = 0; i < symtablelist.activeElement->fundata->depCount; i++)
        {
            // Ak sa funkcia nachadza v liste zavislosti, jej index sa ulozi.
            if(strcmp(symtablelist.activeElement->fundata->dependencies[i]->functionid, token->value) == 0) 
            {
                fundataindex = i;
                break;
            }
        }
        // Ak sa funkcie nenachadza v liste zavislosti, bude pridana.
        if(fundataindex == -1)
        {
            fdepcount += 1;
            fdeplist = realloc(symtablelist.activeElement->fundata->dependencies, fdepcount * sizeof(struct dependencyPair*));
            fdeplist[fdepcount-1] = safeMalloc(sizeof(struct dependencyPair));
            fdeplist[fdepcount-1]->functionid = calloc(strlen(token->value)+1,1);
            fdeplist[fdepcount-1]->paramCount = -1;
            
            strcpy(symtablelist.activeElement->fundata->dependencies[fdepcount-1]->functionid, token->value);
            fundataindex = fdepcount - 1; // Index novej zavislosti je ulozeny.
        }
        
    }
    else
    {
        symtableptr = checkDefined(token, symDLL_GetFirst(&symtablelist));
        for(int i = 0; i < symtableptr->funData->depCount; i++)
        {
            bst_node_t* dependency = bst_search(symDLL_GetFirst(&symtablelist),symtableptr->funData->dependencies[i]->functionid);
            if(dependency != NULL)
            {
                if(dependency->funData->ParamCount != symtableptr->funData->dependencies[i]->paramCount)
                {
                    fprintf(stderr, "Semantic error: Function %s, dependency of %s, previously called with invalid number of arguments.\n", symtableptr->funData->dependencies[i]->functionid, token->value);
                    exit(3);
                }
            }
            else
            {
                fprintf(stderr, "Semantic error: Function %s, dependency of %s not defined.\n", symtableptr->funData->dependencies[i]->functionid, token->value);
                exit(3);
            }
        }
    }
    *token = get_token(SKIP);
    if(token->type == lbracket)
    {
        int paramcount = 0;
        *token = get_token(SKIP);
        if(p_callargs(token, astree, symtableptr, &paramcount))
        {
            if(symtablelist.activeElement->fundata != NULL && fundataindex != -1) // Keď sa parser nachádza v tele funkcie.
            {
                // Ak je funkcia ulozena ale nezhoduje sa pocet parametrov, ukonci program s chybou.
                if(fdeplist[fundataindex]->paramCount != -1 || fdeplist[fundataindex]->paramCount != paramcount)
                {
                    fprintf(stderr, "Semantic error: Dependent function called multiple times with different parameters.\n");
                    exit(3);
                }
                // Inak je ulozeny pocet parametrov.
                fdeplist[fundataindex]->paramCount = paramcount;
            }
            
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
/// @brief Funkcia na volanie argumentov funkcie.
/// @param token Ukazatel na spracovavany token.
/// @param astree Ukazatel na abstraktny syntakticky strom.
/// @param symtableentry Ukazatel na funkciu v tabulke symbolov.
/// @param paramcount Pocet parametrov.
bool p_callargs(token_t *token, ASSnode_t *astree, bst_node_t *symtableentry, int* paramcount)
{
    if(token->type == rbracket)
    {
        if(symtableentry != NULL && symtableentry->funData->ParamCount != -1 && symtableentry->funData->ParamCount != *paramcount)
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

/// @brief Funkcia na spracovanie pokracovania argumentov volania funkcie.
/// @param token Ukazatel na spracovavany token.
/// @param astree Ukazatel na abstraktny syntakticky strom.
/// @param symtableentry Ukazatel na funkciu v tabulke symbolov.
/// @param paramcount Pocet parametrov.
bool p_ncallargs(token_t *token, ASSnode_t *astree, bst_node_t *symtableentry, int* paramcount)
{
    if(token->type == rbracket)
    {
        if(symtableentry != NULL && symtableentry->funData->ParamCount != -1 && symtableentry->funData->ParamCount != *paramcount)
        {
            fprintf(stderr, "Semantic error: Argument count mismatch for %s, expected: %d, got %d.\n", symtableentry->key, symtableentry->funData->ParamCount, *paramcount);
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
/// @brief Funkcia na spracovanie vyrazu alebo konstanty.
/// @param token Ukazatel na spracovavany token.
/// @param astree Ukazatel na abstraktny syntakticky strom.
/// @param symtableentry Ukazatel na funkciu v tabulke symbolov.
/// @param paramcount Pocet parametrov.
/// @return True ak je token validny vyraz alebo konstanta.
bool p_vals(token_t *token, ASSnode_t* astree, bst_node_t *symtableentry, int* paramcount)
{
    
    if(token->type == ID_function)
    {
        bst_node_t* symtableptr = checkDefined(token, symDLL_GetFirst(&symtablelist));
        if(symtableentry != NULL && symtableentry->funData->ParamCount != -1)
        {
            if(symtableentry->funData->ParamCount < *paramcount)
            {
                fprintf(stderr, "Semantic error: Argument count mismatch for %s, expected: %d, got %d.\n", symtableentry->key, symtableentry->funData->ParamCount, *paramcount);
                exit(4);
            }
            if(symtableentry->funData->paramTypes == NULL && symtableentry->funData->ParamCount > 0)
            {

            }
            else if(!checkParam(symtableentry->funData, *paramcount, symtableptr->funData->returnType))
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
                if(oldtoken.type == ID_variable || oldtoken.type == ffloat || oldtoken.type == integer || oldtoken.type == string || oldtoken.type == funnull)
                {
                    if(oldtoken.type == ID_variable)
                        checkDefined(&oldtoken,symtablelist.activeElement->symtable);
                    if(symtableentry != NULL && symtableentry->funData->ParamCount != -1)
                    {   
                        if(symtableentry->funData->ParamCount < *paramcount)
                        {
                            fprintf(stderr, "Semantic error: Argument count mismatch for %s, expected: %d, got %d.\n", symtableentry->key, symtableentry->funData->ParamCount, *paramcount);
                            exit(4);
                        }
                        if(symtableentry->funData->paramTypes == NULL && symtableentry->funData->ParamCount > 0)
                        {

                        }
                        else if(oldtoken.type != ID_variable && !checkParam(symtableentry->funData, *paramcount, oldtoken.type))
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
}
/// @brief Spracuje podmienku if
/// @param token Ukazatel na spracovavany token.
/// @param astree Ukazatel na abstraktny syntakticky strom.
/// @param mainreturn Premenna, ktora oznacuje ci sa analyzator nachadza v hlavnom tele programu (nie vo funkcii).
/// @param rettyp Typ ktory vracia funkcia v ktorej sa if nachadza.
bool p_ifstat(token_t * token, ASSnode_t *astree, bool mainreturn, returnType_t rettyp)
{
    *token = get_token(SKIP);
    if(token->type == lbracket)
    {
        *token = get_token(SKIP);
        if(token->type == ID_function)
        {
            
            astree->left = makeTree(FUNCTIONCALL, NULL, makeLeaf(token));
            if(p_fcall(token, astree->left))
            {
                *token = get_token(SKIP);
                astree->right = makeTree(THEN, NULL, NULL);
                
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
/// @brief Spracuje telo if.
/// @param token Ukazatel na spracovavany token.
/// @param astree Ukazatel na abstraktny syntakticky strom.
/// @param mainreturn Premenna, ktora oznacuje ci sa analyzator nachadza v hlavnom tele programu (nie vo funkcii).
/// @param rettyp Typ ktory vracia funkcia v ktorej sa if nachadza.
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
/// @brief Spracuje telo else.
/// @param token Ukazatel na spracovavany token.
/// @param astree Ukazatel na abstraktny syntakticky strom.
/// @param mainreturn Premenna, ktora oznacuje ci sa analyzator nachadza v hlavnom tele programu (nie vo funkcii).
/// @param rettyp Typ ktory vracia funkcia v ktorej sa else nachadza.
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
/// @brief Spracuje podmienku while.
/// @param token Ukazatel na spracovavany token.
/// @param astree Ukazatel na abstraktny syntakticky strom.
/// @param mainreturn Premenna, ktora oznacuje ci sa analyzator nachadza v hlavnom tele programu (nie vo funkcii).
/// @param rettyp Typ ktory vracia funkcia v ktorej sa while nachadza.
bool p_whilestat(token_t * token, ASSnode_t *astree, bool mainreturn, returnType_t rettyp)
{
    *token = get_token(SKIP);
    if(token->type == lbracket)
    {
        *token = get_token(SKIP);
        if(token->type == ID_function)
        {
            
            astree->left = makeTree(FUNCTIONCALL, NULL, makeLeaf(token));
            if(p_fcall(token, astree->left))
            {
                *token = get_token(SKIP);
                astree->right = makeTree(RYAN_GOSLING, NULL, NULL);
                
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
            
            ASSnode_t *exprptr = NULL;
            exprptr = expr(NULL, token, rbracket, rbracket, 0);
            if(exprptr != NULL)
            {
                astree->left = exprptr;
                
                *token = get_token(SKIP);
                astree->right = makeTree(RYAN_GOSLING, NULL, NULL);
                
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

/// @brief Spracuje telo while.
/// @param token Ukazatel na spracovavany token.
/// @param astree Ukazatel na abstraktny syntakticky strom.
/// @param mainreturn Premenna, ktora oznacuje ci sa analyzator nachadza v hlavnom tele programu (nie vo funkcii).
/// @param rettyp Typ ktory vracia funkcia v ktorej sa while nachadza.
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

/// @brief Funkcie pre deklaraciu funkcii.
/// @param token Ukazatel na spracovavany token.
/// @param astree Ukazatel na abstraktny syntakticky strom.
/// @return True ak je deklaracia funkcie syntakticky spravne, inak ukonci program s chybou.
bool p_fundec(token_t * token, ASSnode_t *astree)
{
    if(token->type == ID_function)
    {
        // Ak existuje funkcia v tabulke symbolov, ukonci program s chybou.
        if(bst_search(symDLL_GetFirst(&symtablelist), token->value) != NULL)
        {
            fprintf(stderr, "Semantic error: Function %s redefinition.\n", token->value);
            exit(3);
        }
        funData_t* newfunc = safeMalloc(sizeof(struct funData));
        newfunc->defined = true;
        newfunc->ParamCount = 0;
        newfunc->depCount = 0;
        newfunc->dependencies = NULL;
        newfunc->paramTypes = NULL;

        astree->left = makeTree(FDEC_DATA, makeTree(FDEC_DATA_INF, makeLeaf(token), NULL), NULL);

        *token = get_token(SKIP);

        // Vytvori novu tabulku symbolov pre funkciu
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

/// @brief Pripravi spracovanie argumentov
/// @param token Ukazatel na spracovavany token.
/// @param astree Ukazatel na abstraktny syntakticky strom.
/// @param newfunc Ukazatel na strukturu dat novo deklarovanej funkcie.
/// @return True ak je deklaracia argumentov syntakticky spravne, inak ukonci program s chybou.
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

/// @brief Funkcia ktora spracuje typ a telo funkcie.
/// @param token Ukazatel na spracovavany token.
/// @param astree Ukazatel na abstraktny syntakticky strom.
/// @param newfunc Ukazatel na strukturu dat novo deklarovanej funkcie.
/// @return True ak typ a telo funkcie su syntakticky spravne, inak ukonci program s chybou.
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
        *token = get_token(SKIP);
        if(p_body(token, false, astree->right, false, translate(astree->left->left->right->Patrick_Bateman->value)))
        {
            // Po prejdeni funkcneho tela, nastavi predoslu tabulku symbolov ako aktivnu a poslednu zahodi.
            symDLL_Previous(&symtablelist);
            symDLL_DeleteLast(&symtablelist);
            return true;
        }
        else
        {
            fprintf(stderr, "Unknown error in function declaration\n");
            exit(2);
        }
    }
    else
    {
        fprintf(stderr, "Syntax error: Expected '{' after function declaration, got '%s'\n", token->value);
        exit(2);
    }
    return true;
}

/// @brief Funkcia kontroluje parameter definicie funkcie.
/// @param token Ukazatel na spracovavany token.
/// @param astree Ukazatel na abstraktny syntakticky strom.
/// @param newfunc Ukazatel na strukturu dat novo deklarovanej funkcie.
/// @return True ak najde koniec zatvorky alebo pokracovanie argumentov, inak ukonci program s chybou.
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
        
        *token = get_token(SKIP);
        if(token->type == ID_variable)
        {
            // Ak uz predtym bolo toto meno premennej pouzite v parametroch, ukonci program s chybou.
            if(isDefined(token, symtablelist.activeElement->symtable))
            {
                fprintf(stderr, "Semantic error: Function parameter %s redefinition.\n", token->value);
                exit(4);
            }
            else
            {
                // Vlozi parameter do tabulky symbolov.
                bst_insert(&(symtablelist.activeElement->symtable), token->value, token->type, NULL);
            }
            astree->left->right = makeLeaf(token);
            
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

/// @brief Funkcia spracuje dalsi parameter deklaracie funkcie.
/// @param token Ukazatel na token.
/// @param astree Ukazatel na abstraktny syntakticky strom.
/// @param newfunc Ukazatel na strukturu dat novo deklarovanej funkcie.
/// @return True ak najde koniec zatvorky alebo pokracovanie argumentov, inak ukonci program s chybou.
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