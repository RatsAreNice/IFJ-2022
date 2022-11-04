#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct token {
  char* type;
  char* value;
} token_t;

token_t* scanner(){
    char* state = "";
    char a = getchar();
    while(a == ' ' || a == '\t' || a == '\n'){          //preskocenie prazdnych znakov
        a = getchar();
    }


    if((a >= 97 && a <= 122) || a == '_' || (a >= 65 && a <= 90)){              //male pismeno / velke pismeno / _
        state = "IDfunkcie";
        ungetc(a,stdin);
    }

    if(a == '$'){
        state = "premenna1";
        ungetc(a,stdin);
    }

    if(a == '"'){
        state = "string1";
        ungetc(a,stdin);
    }

    if(a == '?'){
        state = "type1";
        ungetc(a,stdin);
    }

    if(a == '('){
        token_t* lbracket;
        lbracket->type = "(";
        lbracket->value = NULL;
        return lbracket;
    }

    if(a == ')'){
        token_t* rbracket;
        rbracket->type = ")";
        rbracket->value = NULL;
        return rbracket;
    }

    if(a == '+'){
        token_t* plus;
        plus->type = "+";
        plus->value = NULL;
        return plus;
    }

    if(a == '-'){
        token_t* minus;
        minus->type = "-";
        minus->value = NULL;
        return minus;
    }

    if(a == '*'){
        token_t* multiply;
        multiply->type = "*";
        multiply->value = NULL;
        return multiply;
    }

    if(a == '/'){
        token_t* div;
        div->type = "/";
        div->value = NULL;
        return div;
    }

    if(a == '<'){
        token_t* lt;
        lt->type = "<";
        lt->value = NULL;
        return lt;
    }

    if(a == '>'){
        token_t* mt;
        mt->type = "-";
        mt->value = NULL;
        return mt;
    }

    if(a == ';'){
        token_t* strednik;
        strednik->type = ";";
        strednik->value = NULL;
        return strednik;
    }

    if(a == ','){
        token_t* ciarka;
        ciarka->type = ",";
        ciarka->value = NULL;
        return ciarka;
    }

    if(a >= 48 && a<= 57){
        state = "INT";
        ungetc(a,stdin);
    }

    if(a == '='){
        state = "priradenie";
        ungetc(a,stdin);
    }

    if(a == '!'){
        state = "nidentity1";
        ungetc(a,stdin);
    }

    if(a == EOF){
        token_t* eof;
        eof->type = "EOF";
        eof->value = NULL;
        return eof;
    }


    
    
}

int main(int argc, char const *argv[])
{
    return 0;
}