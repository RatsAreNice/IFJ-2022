#ifndef SCANNER_H
#define SCANNER_H
typedef enum{
    ID_function,        //abc
    ID_variable,        //$abc
    type,               //?abc
    lbracket,           //(
    rbracket,           //)
    lsetbracket,        //{
    rsetbracket,        //}
    plus,               //+
    minus,              //-
    mul,                //*
    division,           ///
    lt,                 //<
    mt,                 //>
    lte,                //<=
    mte,                //>=
    semicolon,          //;
    assign,             //=
    identity,           //===
    nidentity,          //!==
    comma,              //,
    integer,            //56
    string,             //"abc"
    ffloat,             //13.56
    funnull,            //null
    dot,                //. (retazcovy operator)
    colon,              //:
    //klucove slova
    funreturn,          //return
    funelse,            //else
    function,           //function
    funif,              //if
    funvoid,            //void
    funwhile,           //while
    prolog1,            //<?php             [VRATANE PRAZDNEHO ZNAKU ZA]
    prolog2,            //declare(strict_types=1);
    epilog,             //?>                 [ak je za znackou \n, zozerie ho to. (aby sa dalo potom testovat na EOF)]
    eof,       
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
    floating,
    identity2,
    nidentity1,
    identity1,
    identity0,
    nidentity2,
    nidentity0,
    ERROR,
    lt1,
    lt0,
    mt0,
    declare,
} state;

typedef struct token {
  token_type type;
  char* value;
} token_t;

int hextoint(char k);
int pwr(int a, int b);

token_t make_token(token_type typ, char* hodnota);
token_t get_token(int skip);
#endif