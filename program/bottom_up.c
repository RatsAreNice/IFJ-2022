//Autor: Oliver Nemcek <xnemce08>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "bottom_up.h"
#include "semantic.h"

int convert(token_t a){       //konvertuje token na cislo
    switch(a.type){
        case funnull:
            return 0;
        case dot:
            return 1;
        case string:
            return 2;
        case integer:
            return 3;
        case ffloat:
            return 3;
        case ID_variable:
            return 0;
        case plus:
            return 4;
        case minus:
            return 5;
        case mul:
            return 6;
        case division:
            return 7;
        case lt:
            return 8;
        case mt:
            return 8;
        case mte:
            return 8;
        case lte:
            return 8;
        case identity:
            return 9;
        case nidentity:
            return 9;
        case lbracket:
            return 10;
        case rbracket:
            return 11;
        default:
            fprintf(stderr,"neocakavany znak vo vyraze: %d\n" ,a.type);
            exit(2);
    }
}    

unit_t get_input(token_type ** first,char* firstv, token_type ** second,char* secondv, token_t ** token, token_type end, token_type end2,int skip,int* bracketcount){           //ziska token zo vstupu a konvertuje ho
    token_t input;
    unit_t output;
    int b;
    if(*first != NULL){
        input.type = **first;
        input.value = firstv;
        *first = NULL;
    }else if(*second != NULL){
        input.type = **second;
        input.value = secondv;
        *second = NULL;
    }
    else{                           //first aj second su NULL
        input = get_token(skip);
    }

    if(input.type == ID_variable){
        checkDefined(&input,symtablelist.activeElement->symtable);
    }

    if(input.type == end || input.type == end2){
        if(input.type == rbracket){                              //sme na zatvorke a jeden z konecnych symbolov je zatvorka
            if(*bracketcount == 0){
                (*token)->type = input.type;
                b = 12;
            }
            else{
                b = convert(input);
            }
        }
        else{
            (*token)->type = input.type;
            b = 12;
        }

    }
    else{
        b = convert(input);             //z tokenu spravi jemu priradene cislo
    }

    if(b == 10){
        *bracketcount = *bracketcount + 1;
    }
    if(b == 11){
        *bracketcount=*bracketcount-1;
    }
    output.ttyp = b;
    output.vvalue = input;
    output.uzol = NULL;
    return output;
}

unit_t cmp_to_rule(unit_t rs[]){                  //funkcia dostane pravu stranu pravidla. vrati lavu stranu tohto pravidla. (vracia neterminal)
    unit_t neterminal;
    neterminal.ttyp = -30;
    neterminal.uzol = NULL;
    token_t TESTQ;
    TESTQ.type = funvoid;
    TESTQ.value = "";
    neterminal.vvalue = TESTQ;
    if(rs[0].ttyp == 3){                         // <exp> -> i
        neterminal.ttyp = -3;
        neterminal.vvalue = rs[0].vvalue;
        token_t* abc;
        abc = malloc(sizeof(token_t));
        *abc = rs[0].vvalue; 
        neterminal.uzol = makeLeaf(abc);            //vo vytvorenom neterminale bude ukazatel na vytvoreny leaf. v leafe je hodnota terminalu, z ktoreho neterminal vznikol
    }
    else if(rs[0].ttyp == 0){                         //<null> -> null
        neterminal.ttyp = -2;
        neterminal.vvalue = rs[0].vvalue;
        token_t* abc;
        abc = malloc(sizeof(token_t));
        *abc = rs[0].vvalue; 
        neterminal.uzol = makeLeaf(abc);
    }
    else if(rs[0].ttyp == 10 && rs[1].ttyp == -2 && rs[2].ttyp == 11){                  //(<nnull>)  -> null
        neterminal.ttyp = -2;
        neterminal.uzol = rs[1].uzol;
        neterminal.vvalue = rs[1].vvalue;                                           //zatvorky sa v podstate iba odstrania
    }
    else if(rs[0].ttyp == 10 && rs[1].ttyp == -3 && rs[2].ttyp == 11){                    // <exp> -> (<exp>)
        neterminal.ttyp = -3;
        neterminal.uzol = rs[1].uzol;
        neterminal.vvalue = rs[1].vvalue; 
    }
    else if((rs[0].ttyp == -3|| rs[0].ttyp == -2) && rs[1].ttyp == 4 && (rs[2].ttyp == -3 || rs[2].ttyp == -2)){                    // <exp> -> <exp>||<nnull> + <exp>||<nnull>
        neterminal.ttyp = -3;
        neterminal.uzol = makeTree(ADD,rs[0].uzol,rs[2].uzol);
        neterminal.vvalue = rs[1].vvalue;
    }
    else if((rs[0].ttyp == -3|| rs[0].ttyp == -2) && rs[1].ttyp == 5 && (rs[2].ttyp == -3 || rs[2].ttyp == -2)){                    // <exp> -> <exp>||<nnull> - <exp>||<nnull>
        neterminal.ttyp = -3;
        neterminal.uzol = makeTree(SUB,rs[0].uzol,rs[2].uzol);
        neterminal.vvalue = rs[1].vvalue;
    }
    else if((rs[0].ttyp == -3|| rs[0].ttyp == -2) && rs[1].ttyp == 6 && (rs[2].ttyp == -3 || rs[2].ttyp == -2)){                    // <exp> -> <exp>||<nnull> * <exp>||<nnull>
        neterminal.ttyp = -3;
        neterminal.uzol = makeTree(MUL,rs[0].uzol,rs[2].uzol);                                      //vytvori strom s operandom *, naviaze uzly operandov
        neterminal.vvalue = rs[1].vvalue;
    }
    else if((rs[0].ttyp == -3|| rs[0].ttyp == -2) && rs[1].ttyp == 7 && (rs[2].ttyp == -3 || rs[2].ttyp == -2)){                    // <exp> -> <exp>||<nnull> / <exp>||<nnull>
        neterminal.ttyp = -3;
        neterminal.uzol = makeTree(DIV,rs[0].uzol,rs[2].uzol);
        neterminal.vvalue = rs[1].vvalue;
    }
    else if(rs[0].ttyp == 10 && rs[1].ttyp == -4 && rs[2].ttyp == 11){                    // <strexp> -> (<strexp>)
        neterminal.ttyp = -4;
        neterminal.uzol = rs[1].uzol;
        neterminal.vvalue = rs[1].vvalue;
    }
    else if(rs[0].ttyp == 2 && rs[1].ttyp == -1 && rs[2].ttyp == -1){                    // <strexp> -> string
        neterminal.ttyp = -4;
        neterminal.vvalue = rs[0].vvalue;
        token_t* abc;
        abc = malloc(sizeof(token_t));
        *abc = rs[0].vvalue; 
        neterminal.uzol = makeLeaf(abc);
    }
    else if((rs[0].ttyp == -4 || rs[0].ttyp == -2) && rs[1].ttyp == 1 && (rs[2].ttyp == -4 || rs[2].ttyp == -2)){                    // <strexp> -> <strexp>||<nnull> dot <strexp>|<nnull>
        neterminal.ttyp = -4;
        neterminal.uzol = makeTree(CONCAT,rs[0].uzol,rs[2].uzol);
        neterminal.vvalue = rs[1].vvalue;
    }
    else if(rs[0].ttyp == 10 && rs[1].ttyp == -5 && rs[2].ttyp == 11){                    // S -> (S)
        neterminal.ttyp = -5;
        neterminal.uzol = rs[1].uzol;
        neterminal.vvalue = rs[1].vvalue;
    }
    else if((rs[0].ttyp == -3 || rs[0].ttyp == -2) && rs[1].ttyp == 8 && (rs[2].ttyp == -3 || rs[2].ttyp == -2)){                    // S -> <exp>|<nnull> relid <exp>|<nnull>
        neterminal.ttyp = -5;
        neterminal.vvalue = rs[1].vvalue;
        if(rs[1].vvalue.type == lt){
            neterminal.uzol = makeTree(LT,rs[0].uzol,rs[2].uzol);
        }
        else if(rs[1].vvalue.type == mt){
            neterminal.uzol = makeTree(GT,rs[0].uzol,rs[2].uzol);
        }
        else if(rs[1].vvalue.type == mte){
            neterminal.uzol = makeTree(GTE,rs[0].uzol,rs[2].uzol);
        }
        else if(rs[1].vvalue.type == lte){
            neterminal.uzol = makeTree(LTE,rs[0].uzol,rs[2].uzol);
        }
        else{
            printf("chyba vytvarania podstromu");
        }
    }
    else if((rs[0].ttyp == -3 || rs[0].ttyp == -2) && rs[1].ttyp == 9 && (rs[2].ttyp == -3 || rs[2].ttyp == -2)){                    // S -> <exp>|<nnull> relid <exp>|<nnull>
        neterminal.ttyp = -5;
        neterminal.vvalue = rs[1].vvalue;
        if(rs[1].vvalue.type == identity){
            neterminal.uzol = makeTree(EQ,rs[0].uzol,rs[2].uzol);
        }
        else if(rs[1].vvalue.type == nidentity){
            neterminal.uzol = makeTree(NEQ,rs[0].uzol,rs[2].uzol);
        }
        else{
            printf("chyba vytvarania podstromu");
        }
    }
    else if((rs[0].ttyp == -4 || rs[0].ttyp == -2) && rs[1].ttyp == 8 && (rs[2].ttyp == -4 || rs[2].ttyp == -2)){                    // S -> <strexp>|<nnull> relid <strexp>|<nnull>
        neterminal.ttyp = -5;
        neterminal.vvalue = rs[1].vvalue;
        if(rs[1].vvalue.type == lt){
            neterminal.uzol = makeTree(LT,rs[0].uzol,rs[2].uzol);
        }
        else if(rs[1].vvalue.type == mt){
            neterminal.uzol = makeTree(GT,rs[0].uzol,rs[2].uzol);
        }
        else if(rs[1].vvalue.type == mte){
            neterminal.uzol = makeTree(GTE,rs[0].uzol,rs[2].uzol);
        }
        else if(rs[1].vvalue.type == lte){
            neterminal.uzol = makeTree(LTE,rs[0].uzol,rs[2].uzol);
        }
        else{
            printf("chyba vytvarania podstromu");
        }
    }
    else if((rs[0].ttyp == -4 || rs[0].ttyp == -2) && rs[1].ttyp == 9 && (rs[2].ttyp == -4 || rs[2].ttyp == -2)){                    // S -> <strexp>|<nnull> relid <strexp>|<nnull>
        neterminal.ttyp = -5;
        neterminal.vvalue = rs[1].vvalue;
        if(rs[1].vvalue.type == identity){
            neterminal.uzol = makeTree(EQ,rs[0].uzol,rs[2].uzol);
        }
        else if(rs[1].vvalue.type == nidentity){
            neterminal.uzol = makeTree(NEQ,rs[0].uzol,rs[2].uzol);
        }
        else{
            printf("chyba vytvarania podstromu");
        }
    }
    else if((rs[0].ttyp == -3 || rs[0].ttyp == -4) && rs[1].ttyp == 9 && (rs[2].ttyp == -3 || rs[2].ttyp == -4)){                    // S -> <strexp>|<exp> relid <strexp>|<exp>
        neterminal.ttyp = -5;
        neterminal.vvalue = rs[1].vvalue;
        if(rs[1].vvalue.type == identity){
            neterminal.uzol = makeTree(EQ,rs[0].uzol,rs[2].uzol);
        }
        else if(rs[1].vvalue.type == nidentity){
            neterminal.uzol = makeTree(NEQ,rs[0].uzol,rs[2].uzol);
        }
        else{
            printf("chyba vytvarania podstromu");
        }
    }
    else{
        fprintf(stderr,"redukcia retazca ktory nieje na pravej strane ziadneho pravidla - nespravna syntax retazec = %d : %d : %d",rs[0].ttyp,rs[1].ttyp,rs[2].ttyp);
        if((rs[0].ttyp == -2 || rs[0].ttyp == -3 || rs[0].ttyp == -4) && (rs[1].ttyp == 4 || rs[1].ttyp == 5 || rs[1].ttyp == 6 || rs[1].ttyp == 7 ||rs[1].ttyp == 1 || rs[1].ttyp == 8 || rs[1].ttyp == 9) && (rs[2].ttyp == -2 || rs[2].ttyp == -3 || rs[2].ttyp == -4)){
            exit(7);
        }else{
            exit(2);
        }
    }
    return neterminal;
}

ASSnode_t* expr(token_t* first,token_t* second, token_type end, token_type end2, int skip){

    //tabulka
    char prec_t[13][13];
    for(int i = 0;i<=12;i++){
        for(int j = 0; j<=12;j++){
            prec_t[i][j] = 'e';
        }
    }
    prec_t[0][1] = '>';
    prec_t[0][4] = '>';
    prec_t[0][5] = '>';
    prec_t[0][6] = '>';
    prec_t[0][7] = '>';
    prec_t[0][8] = '>';
    prec_t[0][9] = '>';
    prec_t[0][11] = '>';
    prec_t[0][12] = '>';
    prec_t[1][0] = '<';
    prec_t[1][1] = '>';
    prec_t[1][2] = '<';
    prec_t[1][4] = '>';
    prec_t[1][5] = '>';
    prec_t[1][6] = '<';
    prec_t[1][7] = '<';
    prec_t[1][8] = '>';
    prec_t[1][9] = '>';
    prec_t[1][10] = '<';
    prec_t[1][11] = '>';
    prec_t[1][12] = '>';
    prec_t[2][1] = '>';
    prec_t[2][8] = '>';
    prec_t[2][9] = '>';
    prec_t[2][11] = '>';
    prec_t[2][12] = '>';
    prec_t[3][4] = '>';
    prec_t[3][5] = '>';
    prec_t[3][6] = '>';
    prec_t[3][7] = '>';
    prec_t[3][8] = '>';
    prec_t[3][9] = '>';
    prec_t[3][11] = '>';
    prec_t[3][12] = '>';
    prec_t[4][0] = '<';
    prec_t[4][1] = '>';
    prec_t[4][3] = '<';
    prec_t[4][4] = '>';
    prec_t[4][5] = '>';
    prec_t[4][6] = '<';
    prec_t[4][7] = '<';
    prec_t[4][8] = '>';
    prec_t[4][9] = '>';
    prec_t[4][10] = '<';
    prec_t[4][11] = '>';
    prec_t[4][12] = '>';
    for(int i = 0; i<=12;i++){
        prec_t[5][i] = prec_t[4][i];
    }
    prec_t[6][0] = '<';
    prec_t[6][1] = '>';
    prec_t[6][3] = '<';
    prec_t[6][4] = '>';
    prec_t[6][5] = '>';
    prec_t[6][6] = '>';
    prec_t[6][7] = '>';
    prec_t[6][8] = '>';
    prec_t[6][9] = '>';
    prec_t[6][10] = '<';
    prec_t[6][11] = '>';
    prec_t[6][12] = '>';
    for(int i = 0; i<=12;i++){
        prec_t[7][i] = prec_t[6][i];
    }
    prec_t[8][0] = '<';
    prec_t[8][1] = '<';
    prec_t[8][2] = '<';
    prec_t[8][3] = '<';
    prec_t[8][4] = '<';
    prec_t[8][5] = '<';
    prec_t[8][6] = '<';
    prec_t[8][7] = '<';
    prec_t[8][8] = '>';
    prec_t[8][9] = '>';
    prec_t[8][10] = '<';
    prec_t[8][11] = '>';
    prec_t[8][12] = '>';
    for(int i = 0; i<=12;i++){
        prec_t[9][i] = prec_t[8][i];
    }
    prec_t[9][8] = '<';

    prec_t[10][0] = '<';
    prec_t[10][1] = '<';
    prec_t[10][2] = '<';
    prec_t[10][3] = '<';
    prec_t[10][4] = '<';
    prec_t[10][5] = '<';
    prec_t[10][6] = '<';
    prec_t[10][7] = '<';
    prec_t[10][8] = '<';
    prec_t[10][9] = '<';
    prec_t[10][10] = '<';
    prec_t[10][11] = '=';

    prec_t[11][1] = '>';
    prec_t[11][4] = '>';
    prec_t[11][5] = '>';
    prec_t[11][6] = '>';
    prec_t[11][7] = '>';
    prec_t[11][8] = '>';
    prec_t[11][9] = '>';
    prec_t[11][11] = '>';
    prec_t[11][12] = '>';

    for(int i = 0; i<=12;i++){
        prec_t[12][i] = prec_t[10][i];
    }
    prec_t[12][11] = 'e';

    //tabulka
    char* firstv = "TEST";
    char* secondv = "TEST";
    int bracketcount;
    bracketcount = 0;
    unit_t neterminal;
    int endflag = 0;
    char akcia;
    unit_t b;
    token_type *one, *two;
    if (first == NULL){
        one = NULL;
    }
    else{
        one = &(first->type);
        firstv = first->value;
    }
    if (second == NULL){
        two = NULL;
    }
    else{
        two = &(second->type);
        secondv = second->value;
    }
    DLList a;
    unit_t q;       //q = najvyssi terminal na zasobniku
    DLL_Init( &a );
    unit_t dolar;
    dolar.ttyp = 12;
    token_t TESTQ;
    TESTQ.type = funvoid;
    TESTQ.value = "";
    dolar.vvalue = TESTQ;
    dolar.uzol = NULL;
    DLL_InsertFirst( &a , dolar );     //vlozenie $ do zasobniku
    DLL_First(&a);
    unit_t rs[3];
    b = get_input(&one,firstv,&two,secondv,&second,end,end2,skip,&bracketcount);    //b = aktualny symbol na vstupe
    
    while((b.ttyp != 12) || (endflag != 1)){
        
        endflag = 0;
        for(int i=0;i<3;i++){
            rs[i].ttyp = -1;
            rs[i].uzol = NULL;
            rs[i].vvalue = TESTQ;
        } 
        int i = 0;
        DLL_GetValue(&a,&q);

        akcia = prec_t[q.ttyp][b.ttyp];
        if(akcia == '='){
            DLL_InsertLast(&a,b);
            DLL_Last(&a);               //vlozi sa do zoznamu, stane sa aktivnym (kedze to automaticky musi byt najvrchnejsi terminal)
            b = get_input(&one,firstv,&two,secondv,&second,end,end2,skip,&bracketcount);
        }
        else if(akcia == '<'){
            unit_t mensitko;
            mensitko.ttyp = '<';
            mensitko.uzol = NULL;
            mensitko.vvalue = TESTQ;

            DLL_InsertAfter(&a,mensitko);        //za aktivny prvok (najvyssi terminal) sa vlozi <
            DLL_InsertLast(&a,b);           //push b
            DLL_Last(&a);                   //make b active (novy najvrchnejsi terminal)
            b = get_input(&one,firstv,&two,secondv,&second,end,end2,skip,&bracketcount);       //precitaj dalsi symbol b zo vstupu
        }
        else if(akcia == '>'){
            DLL_Last(&a);
            unit_t vacsitko;
            vacsitko.ttyp = '>';
            vacsitko.uzol = NULL;
            vacsitko.vvalue = TESTQ;

            DLL_InsertAfter(&a,vacsitko);
            DLL_GetValue(&a,&q);
            while(q.ttyp != '<'){                //posuvaj sa v zozname dolava pokial nenarazis na <
                DLL_Previous(&a);
                DLL_GetValue(&a,&q);
            }
            DLL_Next(&a);
            DLL_GetValue(&a,&q);
            while(q.ttyp != '>'){
                rs[i] = q;                //do pola rs sa uklada vyraz medzi <>
                DLL_Next(&a);
                DLL_GetValue(&a,&q);
                i++;
            }

            neterminal = cmp_to_rule(rs);       //v premmenej neterminal je neterminal ktory musime vlozit na zasobnik po tom co odstranime symboly medzi <>, vratane '<' '>' samotnych
            //printf("neterminal uzol value : %s",neterminal.uzol->Patrick_Bateman->value);
            DLL_Last(&a);                       
            while(q.ttyp != '<'){
                DLL_Previous(&a);
                DLL_DeleteAfter(&a);
                DLL_GetValue(&a,&q);
            }
            DLL_DeleteLast(&a);                 //cely retazec <...> je vymazany
            DLL_InsertLast(&a,neterminal);      //vlozenie neterminalu namiesto <...>
        }
        else if(akcia == 'e'){
            fprintf(stderr, "pre kombinaciu zasobnik / vstup nieje v precedencnej tabulke symbol - chybna syntax");
            if(q.ttyp == 1 || q.ttyp == 4 ||q.ttyp == 5 ||q.ttyp == 6 || q.ttyp == 7 || b.ttyp == 1 || b.ttyp == 4 ||b.ttyp == 5 ||b.ttyp == 6 || b.ttyp == 7){
                exit(7);
            }
            else{
                exit(2);
            }
        }

        DLL_Last(&a);           //kontrola ci zasobnik obsahuje iba $S
        DLL_GetValue(&a, &q);   
        if(q.ttyp == -5 || q.ttyp == -3 || q.ttyp == -4 || q.ttyp == - 2){
            DLL_Previous(&a);
            DLL_GetValue(&a, &q);
            if(q.ttyp == 12){
                endflag = 1;                //ak ano, nastavi sa endflag na 1
            }
        }

        DLL_Last(&a);
        DLL_GetValue(&a,&q);
        while(q.ttyp <0 || q.ttyp == '<' || q.ttyp == '>' || q.ttyp == '='){             //od posledneho chod cez vsetky prvky na zasobniku, pokial nenarazis na terminal
            DLL_Previous(&a);
            DLL_GetValue(&a,&q);
        }                                                           //najpravsi terminal je aktivny
    }

    DLL_Last(&a);
    DLL_GetValue(&a,&q);
    

    //strom = q.uzol
    return q.uzol;
}