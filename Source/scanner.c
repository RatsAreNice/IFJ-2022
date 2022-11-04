//todo: komentare, string, type, priradenia/identity, EOF

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum{
    ID_function,
    ID_variable,
    string,
    type,
    lbracket,
    rbracket,
    plus,
    minus,
    mul,
    division,
    lt,
    mt,
    semicolon,
    integer,
    assign,
    identity,
    nidentity,
    comma,
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
    priradenie,
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

token_t make_token(token_type typ, char* hodnota){
    token_t lexem;
    lexem.type = typ;
    lexem.value = hodnota;
    return lexem;
}

token_t get_token(){
    state state = start;
    char a;
    char* str;
    char* temp;
    int i;
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
                    return make_token(lbracket, NULL);
                }
                else if(a == ')'){
                    return make_token(rbracket, NULL);
                }
                else if(a == '+'){
                    return make_token(plus, NULL);
                }
                else if(a == '-'){
                    return make_token(minus, NULL);
                }
                else if(a == '*'){
                    return make_token(mul, NULL);
                }
                else if(a == '/'){
                    return make_token(division, NULL);
                }
                else if(a == '<'){
                    return make_token(lt, NULL);
                }
                else if(a == '>'){
                    return make_token(mt, NULL);
                }
                else if(a == ';'){
                    return make_token(semicolon, NULL);
                }
                else if(a == ','){
                    return make_token(comma, NULL);
                }
                else if(a >= 48 && a<= 57){
                    state = INT;
                }
                else if(a == '='){
                    state = priradenie;
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
                return make_token(ID_function, str);
                break;
            
            case premenna1:
                a = getchar();
                i = 0;
                if( (a != '_') && (a < 65 || (a >= 91 && a <= 96) || a >= 123) ){
                    fprintf(stderr,"chybna synatax premmenej [$]");
                    exit(51); 
                }
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
                state = ERROR;
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

        }
    }

    
    
}

int main(int argc, char const *argv[]){
    token_t a;
    a = get_token();
    printf("typ: %d , value: %s \n", a.type,a.value);
    a = get_token();
    printf("typ: %d , value: %s \n", a.type,a.value);
    a = get_token();
    printf("typ: %d , value: %s \n", a.type,a.value);
}