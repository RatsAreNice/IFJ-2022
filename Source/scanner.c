//todo: komentare, EOF, prolog,
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum{
    ID_function,        //abc
    ID_variable,        //$abc
    string,             //"abc"
    type,               //?abc
    lbracket,           //(
    rbracket,           //)
    plus,               //+
    minus,              //-
    mul,                //*
    division,           ///
    lt,                 //<
    mt,                 //>
    semicolon,          //;
    integer,            //56
    assign,             //=
    identity,           //===
    nidentity,          //!==
    comma,              //,
    //klucove slova
    funreturn,          //return
    funelse,            //else
    function,           //function
    funif,              //if
    funnull,            //null
    funvoid,            //void
    funwhile,           //while
    eof         
} token_type;             //typ lexemu
typedef enum{       //stavy automatu
    start,
    IDfunkcie,
    premenna1,
    premenna2,
    string1,
    string2,
    type1,
    INT,
    identity2,
    nidentity1,
    identity1,
    identity0,
    nidentity2,
    nidentity0,
    ERROR,
} state;

typedef struct token {
  token_type type;
  char* value;
} token_t;

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

token_t get_token(){
    state state = start;
    char a;
    char b;
    char c;
    char* str;
    char* temp;
    int i;
    int z;
    while(state != ERROR){    
        switch(state){
            case start:
                a = getchar();
                while(a == ' ' || a == '\t' || a == '\n'){          //preskocenie prazdnych znakov + komentare
                    a = getchar();
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
                else if(a == ')'){
                    return make_token(rbracket, ")");
                }
                else if(a == '+'){
                    return make_token(plus, "+");
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
                    return make_token(lt, "<");
                }
                else if(a == '>'){
                    return make_token(mt, ">");
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
                else{
                    state = ERROR;
                }
                break;
            
            
            case IDfunkcie:
                str = "";
                i=0;
                while((a >= 97 && a <= 122) || a == '_' || (a >= 65 && a <= 90) || a >= 48 && a<= 57){                      
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


                return make_token(ID_function, str);                    //ak to nieje klucove slovo, je to ID funkcie
                break;
            
            case premenna1:
                a = getchar();
                i = 0;
                if( (a != '_') && (a < 65 || (a >= 91 && a <= 96) || a >= 123) ){
                    fprintf(stderr,"chybna synatax premmenej [$]");
                    exit(51); 
                }
                else{
                    state = premenna2;                  //je to male pismeno/velke pismeno/_
                }
                break;
            case premenna2:
                str = "$";
                while((a >= 97 && a <= 122) || a == '_' || (a >= 65 && a <= 90) || a >= 48 && a<= 57){                      
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
                while(a >= 48 && a <=  57){                      
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
                return make_token(integer, str);
                break;

            case string1:
                z = 0;
                str = "";
                i = 0;
                a=getchar();
                while(a != '"' && a > 31 && a != '$'){      //osetrit skoncenie dolarom / <31 a to ze druhy znak nieje ani jeden esc. sequence / esc seq na oktalove
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
                                printf("tets");
                            } else{
                                cflag = 1;              //inak sa posledny nacitany znak znova spracuje (mohlo by ist o " alebo nepovoleny znak)
                            }
                        }
                        else{                           //'\'a
                            size_t len = strlen(str);
                            char *str2 = malloc(len + 1 + 2);
                            strcpy(str2, str);
                            if(i>0){
                                free(str);
                            }
                            str2[len] = 92;
                            str2[len + 1] = a;
                            str2[len + 2] = '\0';
                            str = str2;
                            a = getchar();
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
                    exit(51);
                }
                break;

            case string2:
                return make_token(string, str);
            break;

            case type1:
                str = "?";
                i=0;
                a = getchar();
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
                    exit(51);
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
                    exit(51);
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
                    exit(51);
                }
                else{
                    state = nidentity2;
                }
                break;
            case nidentity2:
                a = getchar();
                if(a != '='){
                    fprintf(stderr, "chybna syntax neidentity [!=]");
                    exit(51);
                }
                else{
                    state = nidentity0;
                }
                break;
            case nidentity0:
                return make_token(nidentity, "!==");
                break;
        }
    }

    
    
}

int main(int argc, char const *argv[]){
    token_t a;
    a = get_token();
    printf("typ: %d , value: %s \n", a.type,a.value);
    a = get_token();
    printf("typ: %d , value: %s \n", a.type,a.value);
}