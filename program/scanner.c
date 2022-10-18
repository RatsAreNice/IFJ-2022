//Autor : Oliver Nemcek <xnemce08> , Martin Packa <xpacka00>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "scanner.h"

int hextoint(char k){
    int a = k;
    if(a >= 48 && a <= 57){
        return a-48;
    }
    if(a >= 65 && a<= 70){
        return a-55;
    }
    if(a >= 97 && a<= 102){
        return a-87;
    }
    return -1;
}

int pwr(int a, int b){
    int x = 1;
    for(int i = 0; i < b; i++){
        x = x * a;
    }
    return x;
}

token_t make_token(token_type typ, char* hodnota){
    token_t lexem;
    lexem.type = typ;
    lexem.value = hodnota;
    return lexem;
}

token_t get_token(int skip){
    state state = start;
    char a;
    char b;
    char c;
    char* str;
    int i;
    int z;
    while(1){    
        switch(state){
            case start:
                a = getchar();

                if(skip == 1){ // mod na zaciatku -> nic nepreskakuje, ocakava prolog1.
                    if(a == '<')
                    {
                        a = getchar();
                        if(a == '?'){
                            a = getchar();
                            if(a != 'p'){
                                fprintf(stderr, "program nezacina prologom");
                                exit(1);
                            }
                            a = getchar();
                            if(a != 'h'){
                                fprintf(stderr, "program nezacina prologom");
                                exit(1);
                            }
                            a = getchar();
                            if(a != 'p'){
                                fprintf(stderr, "program nezacina prologom");
                                exit(1);
                            }
                            a = getchar();
                            if(!isspace(a) && a != '/'){
                                fprintf(stderr, "program nezacina prologom");
                                exit(1);
                            }
                            ungetc(a, stdin);
                            //*skip = 0;
                            return make_token(prolog1,"<?php ");
                        }
                        else
                        {
                            fprintf(stderr, "program nezacina prologom");
                            exit(2);
                        }
                    }
                    else if (a == EOF)
                    {
                        return make_token(eof, "EOF");
                    }
                    else
                    {
                        fprintf(stderr, "Token mimo rozmedzia prologu a epilogu.");
                        exit(2);
                    }
                }
                else if(skip == 0){ // mod po prologu -> preskakuje whitespaces a komentare.
                    while(isspace(a) || a == '/'){          //preskocenie prazdnych znakov a komentarov
                        if(a == '/'){
                            a = getchar();
                            if(a == '/'){
                                while(a != '\n' && a != EOF){          //riadkove komentare
                                    a = getchar();
                                }
                                if(a == EOF){
                                    ungetc(a,stdin);
                                }
                            }else if(a == '*'){
                                b = a;
                                a = getchar();
                                while((a != '/' || b != '*') && a != EOF){
                                    b = a;
                                    a = getchar();
                                }
                                if(a == EOF){
                                    fprintf(stderr, "neukonceny blokovy komentar");
                                    exit(1);
                                }
                            }
                            else{
                                ungetc(a, stdin);
                                a = '/';
                                break;
                            }
                        }
                        a = getchar();
                    }
                }
                if((a >= 97 && a <= 122) || a == '_' || (a >= 65 && a <= 90)){              //male pismeno / velke pismeno / _
                    state = IDfunkcie;
                }
                else if(a == '$'){
                    state = premenna1;
                }
                else if(a == '"'){
                    state = string1;
                }
                else if(a == '?'){
                    state = type1;
                }
                else if(a == '('){
                    return make_token(lbracket, "(");          //preslo by sa do stavu "(" kde by sa iba vytvoril token lbracket. pre zjednodusenie sa do stavu neprechadza a token sa vytvori priamo
                }
                else if(a == ':'){
                    return make_token(colon, ":");
                }
                else if(a == ')'){
                    return make_token(rbracket, ")");
                }
                else if(a == '+'){
                    return make_token(plus, "+");
                }
                else if(a == '.'){
                    return make_token(dot, ".");
                }
                else if(a == '-'){
                    return make_token(minus, "-");
                }
                else if(a == '*'){
                    return make_token(mul, "*");
                }
                else if(a == '/'){
                    return make_token(division, "/");
                }
                else if(a == '<'){
                    state = lt0;
                }
                else if(a == '>'){
                    state = mt0;
                }
                else if(a == ';'){
                    return make_token(semicolon, ";");
                }
                else if(a == ','){
                    return make_token(comma, ",");
                }
                else if(a >= 48 && a<= 57){
                    state = INT;
                }
                else if(a == '='){
                    state = identity1;
                }
                else if(a == '!'){
                    state = nidentity1;
                }
                else if(a == EOF){
                    return make_token(eof, NULL);
                }
                else if(a == '{'){
                    return make_token(lsetbracket, "{");
                }
                else if(a == '}'){
                    return make_token(rsetbracket, "}");
                }
                else{
                    state = ERROR;
                }
                break;
            
            
            case IDfunkcie:
                str = "";
                i=0;
                while((a >= 97 && a <= 122) || a == '_' || (a >= 65 && a <= 90) || (a >= 48 && a<= 57)){                      
                    size_t len = strlen(str);
                    char *str2 = malloc(len + 1 + 1);
                    strcpy(str2, str);
                    if(i>0){
                        free(str);
                    }
                    str2[len] = a;
                    str2[len + 1] = '\0';
                    str = str2;
                    a = getchar();
                    i++;
                }
                if(i>0){
                    ungetc(a, stdin);
                }

                //testy na klucove slova
                if(strcmp(str,"string") == 0 || strcmp(str,"float") == 0 || strcmp(str,"int") == 0){
                    return make_token(type, str);
                }
                if(strcmp(str,"return") == 0){
                    return make_token(funreturn, str);
                }
                if(strcmp(str,"else") == 0){
                    return make_token(funelse, str);
                }
                if(strcmp(str,"function") == 0){
                    return make_token(function, str);
                }
                if(strcmp(str,"if") == 0){
                    return make_token(funif, str);
                }
                if(strcmp(str,"null") == 0){
                    return make_token(funnull, str);
                }
                if(strcmp(str,"void") == 0){
                    return make_token(funvoid, str);
                }
                if(strcmp(str,"while") == 0){
                    return make_token(funwhile, str);
                }
                if(strcmp(str,"declare") == 0){
                    state = declare;
                    break;
                }


                return make_token(ID_function, str);                    //ak to nieje klucove slovo, je to ID funkcie
                break;
            
            case premenna1:
                a = getchar();
                i = 0;
                if( (a != '_') && (a < 65 || (a >= 91 && a <= 96) || a >= 123) ){
                    fprintf(stderr,"chybna synatax premmenej [$]");
                    exit(1); 
                }
                else{
                    state = premenna2;                  //je to male pismeno/velke pismeno/_
                }
                break;
            case premenna2:
                str = "$";
                while((a >= 97 && a <= 122) || a == '_' || (a >= 65 && a <= 90) || (a >= 48 && a<= 57)){                      
                    size_t len = strlen(str);
                    char *str2 = malloc(len + 1 + 1);
                    strcpy(str2, str);
                    if(i>0){
                        free(str);
                    }
                    str2[len] = a;
                    str2[len + 1] = '\0';
                    str = str2;
                    a = getchar();
                    i++;
                }
                if(i>0){
                    ungetc(a, stdin);
                }
                return make_token(ID_variable, str);
                break;

            case INT:
                str = "";
                i = 0;
                z = 0;
                while((a >= 48 && a <=  57) || a == '.' || a == 'e' || a == 'E'){
                    size_t len = strlen(str);
                    char *str2 = malloc(len + 1 + 1);
                    strcpy(str2, str);
                    if(i>0){
                        free(str);
                    }
                    str2[len] = a;
                    str2[len + 1] = '\0';
                    str = str2;
                    a = getchar();
                    i++;
                    if(a == '.' || a == 'e' || a == 'E'){
                        state = floating;
                        break;     
                    }
                }
                if(i>0 && state != floating){
                    ungetc(a, stdin);
                }
                if(state == floating){
                    break;
                }
                return make_token(integer, str);
                break;

            case floating:
                if(a == '.'){
                    a = getchar();
                    if(!(a >= 48 && a <=  57)){                                         //zisti ci dalsi znak je cislovka
                        fprintf(stderr,"chybajuca cislovka za desatinnou bodkou");
                        exit(1);
                    }

                    size_t len = strlen(str);
                    char *str2 = malloc(len + 1 + 1);
                    strcpy(str2, str);
                    if(i>0){
                        free(str);
                    }
                    str2[len] = '.';
                    str2[len + 1] = '\0';
                    str = str2;                                                             //pridaj do stringu '.'

                    while((a >= 48 && a <=  57)){                                           //pridavaj do stringu vsetky cisla ktore nasleduju
                        size_t len = strlen(str);
                        char *str2 = malloc(len + 1 + 1);
                        strcpy(str2, str);
                        if(i>0){
                            free(str);
                        }
                        str2[len] = a;
                        str2[len + 1] = '\0';
                        str = str2;
                        a = getchar();
                        i++;
                    }


                    if(a == 'e' || a == 'E'){                                               //zisti ci za cislami je este 'e'/'E'
                        a = getchar();
                        if(a == '+' || a == '-'){
                            size_t len = strlen(str);
                            char *str2 = malloc(len + 1 + 2);
                            strcpy(str2, str);
                            if(i>0){
                                free(str);
                            }
                            str2[len] = 'e';
                            str2[len + 1] = a;                                              
                            str2[len + 2] = '\0';
                            str = str2;
                            a = getchar();
                        }
                        else{
                            size_t len = strlen(str);
                            char *str2 = malloc(len + 1 + 1);
                            strcpy(str2, str);
                            if(i>0){
                                free(str);
                            }
                            str2[len] = 'e';
                            str2[len + 1] = '\0';                                                       //pridaj do stringu 'e' a pripadne znamienko
                            str = str2;
                        }
                        if(!(a >= 48 && a <=  57)){
                            fprintf(stderr,"chybajuca cislovka za e");
                            exit(1);
                        }
                        while((a >= 48 && a <=  57)){           //nacitaju sa cislovky za e
                            size_t len = strlen(str);
                            char *str2 = malloc(len + 1 + 1);
                            strcpy(str2, str);
                            if(i>0){
                                free(str);
                            }
                            str2[len] = a;
                            str2[len + 1] = '\0';
                            str = str2;
                            a = getchar();
                            i++;
                        }
                        ungetc(a,stdin);
                        return make_token(ffloat,str);

                    }
                    else{
                        ungetc(a,stdin);
                        return make_token(ffloat,str);                                              
                    }
                }
                else{                                                               //za celym cislom ide priamo e
                    a = getchar();
                        if(a == '+' || a == '-'){
                            size_t len = strlen(str);
                            char *str2 = malloc(len + 1 + 2);
                            strcpy(str2, str);
                            if(i>0){
                                free(str);
                            }
                            str2[len] = 'e';
                            str2[len + 1] = a;
                            str2[len + 2] = '\0';
                            str = str2;
                            a = getchar();
                        }
                        else{
                            size_t len = strlen(str);
                            char *str2 = malloc(len + 1 + 1);   
                            strcpy(str2, str);
                            if(i>0){
                                free(str);
                            }
                            str2[len] = 'e';
                            str2[len + 1] = '\0';                                       //pridaj do stringu e a pripadne znamienko
                            str = str2;
                        }
                        if(!(a >= 48 && a <=  57)){                                     //otestuj ci za e je nejaka cislovka
                            fprintf(stderr,"chybajuca cislovka za e");
                            exit(1);
                        }
                        while((a >= 48 && a <=  57)){           //nacitaju sa cislovky za e
                            size_t len = strlen(str);
                            char *str2 = malloc(len + 1 + 1);
                            strcpy(str2, str);
                            if(i>0){
                                free(str);
                            }
                            str2[len] = a;
                            str2[len + 1] = '\0';
                            str = str2;
                            a = getchar();
                            i++;
                        }
                        ungetc(a,stdin);
                        return make_token(ffloat,str);
                }
            break;


            case string1:
                z = 0;
                str = "";
                i = 0;
                a=getchar();
                while(a != '"' && a > 31 && a != '$'){
                    int cflag = 0;
                    z = 0;


                    if( a == 92){                           //osetrenie escape sekvencii
                        a = getchar();
                        if(a == 'n'){
                            a = '\n';
                        }
                        else if(a == 't'){
                            a = '\t';
                        }
                        else if(a == '"'){
                            a = '"';
                        }
                        else if(a == '$'){
                            a = '$';
                        }
                        else if(a == 92){
                            a = 92;
                        }
                        else if(a == 'x'){
                            for(int j = 1; j >= 0; j--){
                                a = getchar();
                                if((a <= 57 && a >= 48) || (a >= 65 && a<= 70) || (a >= 97 && a <= 102)){       //hexadecimalna cislovka
                                    z = z + hextoint(a) * pwr(16,j);
                                    b = a;
                                }
                                else{
                                    if(j == 1){         //prvy znak neni hexadecimalna cislovka : do stringu treba pridat \x
                                        size_t len = strlen(str);
                                        char *str2 = malloc(len + 1 + 2);
                                        strcpy(str2, str);
                                        if(i>0){
                                            free(str);
                                        }
                                        str2[len] = 92;
                                        str2[len + 1] = 'x';
                                        str2[len + 2] = '\0';
                                        str = str2;
                                        z = -1;
                                        i++;
                                        break;
                                    }
                                    else{               //druhy znak neni hexadecimalna cislovka : do stringu treba pridat \xb
                                        size_t len = strlen(str);
                                        char *str2 = malloc(len + 1 + 3);
                                        strcpy(str2, str);
                                        if(i>0){
                                            free(str);
                                        }
                                        str2[len] = 92;
                                        str2[len + 1] = 'x';
                                        str2[len + 2] = b;
                                        str2[len + 3] = '\0';
                                        str = str2;
                                        z = -1;
                                        i++;
                                        break;
                                    }
                                }
                            }
                            if(z!= -1){                 //ak sa jednalo o escape sekvenciu \xdd pridava sa iba jeden znak, a to vypocitane a 
                                a = z; 
                            }
                            else{
                                cflag = 1;
                            }

                        }else if (a >= 48 && a <= 55){                                      // \oktalove cislo
                            z = 0;
                            ungetc(a, stdin);
                            for(int j = 2; j >= 0; j--){
                                a = getchar();
                                if(a >= 48 && a <= 55){       //hexadecimalna cislovka
                                    z = z + (a-48)*pwr(8,j);
                                    c = b;
                                    b = a;
                                }
                                else{
                                    if(j == 2){         //prvy znak neni oktalova cislovka : do stringu treba pridat '\'
                                        size_t len = strlen(str);
                                        char *str2 = malloc(len + 1 + 1);
                                        strcpy(str2, str);
                                        if(i>0){
                                            free(str);
                                        }
                                        str2[len] = 92;
                                        str2[len + 1] = '\0';
                                        str = str2;
                                        z = -1;
                                        i++;
                                        break;
                                    }
                                    else if(j == 1){               //druhy znak neni oktalova cislovka : do stringu treba pridat \b
                                        size_t len = strlen(str);
                                        char *str2 = malloc(len + 1 + 2);
                                        strcpy(str2, str);
                                        if(i>0){
                                            free(str);
                                        }
                                        str2[len] = 92;
                                        str2[len + 1] = b;
                                        str2[len + 2] = '\0';
                                        str = str2;
                                        z = -1;
                                        i++;
                                        break;
                                    }else{               //posledny znak neni oktalova cislovka : do stringu treba pridat \cb
                                        size_t len = strlen(str);
                                        char *str2 = malloc(len + 1 + 3);
                                        strcpy(str2, str);
                                        if(i>0){
                                            free(str);
                                        }
                                        str2[len] = 92;
                                        str2[len + 1] = c;
                                        str2[len + 2] = b;
                                        str2[len + 3] = '\0';
                                        str = str2;
                                        z = -1;
                                        i++;
                                        break;
                                    }
                                }
                            }
                            if(z!= -1){                 //ak sa jednalo o escape sekvenciu \ddd pridava sa iba jeden znak, a to vypocitane a 
                                a = z; 
                            } else{
                                cflag = 1;              //inak sa posledny nacitany znak znova spracuje (mohlo by ist o " alebo nepovoleny znak)
                            }
                        }
                        else{                           //'\'a
                            size_t len = strlen(str);
                            char *str2 = malloc(len + 1 + 1);
                            strcpy(str2, str);
                            if(i>0){
                                free(str);
                            }
                            str2[len] = 92;
                            //str2[len + 1] = a;
                            str2[len + 1] = '\0';
                            str = str2;
                            //a = getchar();
                            i++;
                            cflag = 1;
                        }
                    }

                    if(cflag != 1){
                    size_t len = strlen(str);
                    char *str2 = malloc(len + 1 + 1);
                    strcpy(str2, str);
                    if(i>0){
                        free(str);
                    }
                    str2[len] = a;
                    str2[len + 1] = '\0';
                    str = str2;
                    a = getchar();
                    i++;
                    }
                }

                if(a == '"'){
                    state = string2;
                }else{
                    fprintf(stderr, "retazec neskoncil na \", neplatny znak v retazci");
                    exit(1);
                }
                break;

            case string2:
                return make_token(string, str);
            break;

            case type1:
                str = "?";
                i=0;
                a = getchar();
                if(a == '>'){                   //nasiel sa epilog ?>
                    a = getchar();
                    if(a != '\r'){              
                        ungetc(a, stdin);
                    }                           //ak nasledujuci znak je prvy znak windows newlinu, skonzumuje sa
                    a = getchar();
                    if(a != '\n'){  
                        ungetc(a, stdin);
                    }                           //ak nasledujuci znak je druhy znak windows newlinu/jediny znak linux newlinu, skonzumuje sa
                    //*skip=2; // scanner sa prepne do post-epilog modu.
                    return make_token(epilog,"?>");
                    break;
                }

                while(a >= 97 && a <= 122){                      
                    size_t len = strlen(str);
                    char *str2 = malloc(len + 1 + 1);
                    strcpy(str2, str);
                    if(i>0){
                        free(str);
                    }
                    str2[len] = a;
                    str2[len + 1] = '\0';
                    str = str2;
                    a = getchar();
                    i++;
                    if(strcmp(str,"?int") == 0 || strcmp(str,"?float") == 0 || strcmp(str,"?string") == 0){
                        break;
                    }
                }
                ungetc(a, stdin);
                if(strcmp(str,"?int") == 0 || strcmp(str,"?float") == 0 || strcmp(str,"?string") == 0){
                    return make_token(type, str);
                }
                else{
                    fprintf(stderr, "chybna syntax typu [?]");
                    exit(1);
                }
                break;
            case identity1:
                a = getchar();
                if(a != '='){
                    ungetc(a, stdin);
                    return make_token(assign, "=");
                }
                else{
                    state = identity2;
                }
                break;
            case identity2:
                a = getchar();
                if(a != '='){
                    fprintf(stderr, "chybna syntax identity/priradenia [==]");
                    exit(1);
                }
                else{
                    state = identity0;
                }
                break;
            case identity0:
                return make_token(identity, "===");
                break;
            case nidentity1:
                a = getchar();
                if(a != '='){
                    fprintf(stderr, "chybna syntax neidentity [!=]");
                    exit(1);
                }
                else{
                    state = nidentity2;
                }
                break;
            case nidentity2:
                a = getchar();
                if(a != '='){
                    fprintf(stderr, "chybna syntax neidentity [!=]");
                    exit(1);
                }
                else{
                    state = nidentity0;
                }
                break;
            case nidentity0:
                return make_token(nidentity, "!==");
                break;
            case ERROR:
                fprintf(stderr, "chybna syntax");
                exit(1);
                break;
            case mt0:
                a = getchar();
                if(a != '='){
                    ungetc(a, stdin);
                    return make_token(mt, ">");
                }
                else{
                    return make_token(mte, ">=");
                }
                break;
            case lt0:
                a = getchar();
                if(a != '?' && a != '='){
                    ungetc(a, stdin);
                    return make_token(lt, "<");
                }
                else if(a == '?'){
                    state = lt1;
                }
                else if(a == '='){
                    return make_token(lte, "<=");
                }
                break;
            case lt1:
                a = getchar();
                if(a != 'p'){
                    ungetc(a, stdin);
                    ungetc('?', stdin);
                    return make_token(lt, "<");
                }
                a = getchar();
                if(a != 'h'){
                    fprintf(stderr, "chybna znacka <?php");
                    exit(1);
                }
                a = getchar();
                if(a != 'p'){
                    fprintf(stderr, "chybna znacka <?php");
                    exit(1);
                }
                a = getchar();
                if(a != ' ' && a != '\n' && a != '\t' && a != '\r'){                 
                    fprintf(stderr, "chybna znacka <?php");
                    exit(1);
                }
                return make_token(prolog1,"<?php ");
                break;
            case declare:
                str = "";
                i=0;
                a = getchar();
                while(a != ';' && a != EOF){                      
                    size_t len = strlen(str);
                    char *str2 = malloc(len + 1 + 1);
                    strcpy(str2, str);
                    if(i>0){
                        free(str);
                    }
                    str2[len] = a;
                    str2[len + 1] = '\0';
                    str = str2;
                    a = getchar();
                    i++;
                }
                if(strcmp(str,"(strict_types=1)") == 0){
                    return make_token(prolog2, "declare(strict_types=1);");
                }
                else{
                    fprintf(stderr, "chybna znacka declare(strict_types=1);");
                    exit(1);
                }
                break;
        }
    }
}