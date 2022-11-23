#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "c206.c"
#include "bottom_up.h"

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
            return 3;
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
            fprintf(stderr,"neocakavany znak vo vyraze: %s\n" ,a.value);
            exit(2);
    }
}    

int get_input(token_t** first, token_t** second,token_type end,int skip){           //ziska token zo vstupu a konvertuje ho
    token_t input;
    int b;
    if(*first != NULL){
        input = **first;
        *first = NULL;
    }else if(*second != NULL){
        input = **second;
        *second = NULL;
    }
    else{                           //first aj second su NULL
        input = get_token(skip);
    }

    if(input.type == end){
        b = 12;
    }
    else{
        b = convert(input);             //z tokenu spravi jemu priradene cislo
    }
    //printf("get_input called. Returning %d. \n", b);
    return b;
}

int cmp_to_rule(int rs[]){                  //funkcia dostane pravu stranu pravidla. vrati lavu stranu tohto pravidla. (vracia neterminal reprezentovany INTom)
    if(rs[0] == 3){                         // <exp> -> i
        return -3;
    }
    if(rs[0] == 0){                         //<null> -> null
        return -2;
    }
    if(rs[0] == 10 && rs[1] == -2 && rs[2] == 11){                  //(<nnull>)  -> null
        return -2;
    }
    if(rs[0] == 10 && rs[1] == -3 && rs[2] == 11){                    // <exp> -> (<exp>)
        return -3;
    }
    if((rs[0] == -3|| rs[0] == -2) && rs[1] == 4 && (rs[2] == -3 || rs[2] == -2)){                    // <exp> -> <exp>||<nnull> + <exp>||<nnull>
        return -3;
    }
    if((rs[0] == -3|| rs[0] == -2) && rs[1] == 5 && (rs[2] == -3 || rs[2] == -2)){                    // <exp> -> <exp>||<nnull> - <exp>||<nnull>
        return -3;
    }
    if((rs[0] == -3|| rs[0] == -2) && rs[1] == 6 && (rs[2] == -3 || rs[2] == -2)){                    // <exp> -> <exp>||<nnull> * <exp>||<nnull>
        return -3;
    }
    if((rs[0] == -3|| rs[0] == -2) && rs[1] == 7 && (rs[2] == -3 || rs[2] == -2)){                    // <exp> -> <exp>||<nnull> / <exp>||<nnull>
        return -3;
    }
    if(rs[0] == 10 && rs[1] == -4 && rs[2] == 11){                    // <strexp> -> (<strexp>)
        return -4;
    }
    if(rs[0] == 2 && rs[1] == -1 && rs[2] == -1){                    // <strexp> -> string
        return -4;
    }
    if((rs[0] == -4 || rs[0] == -2) && rs[1] == 1 && (rs[2] == -4 || rs[2] == -2)){                    // <strexp> -> <strexp>||<nnull> dot <strexp>|<nnull>
        return -4;
    }
    if(rs[0] == 10 && rs[1] == -5 && rs[2] == 11){                    // S -> (S)
        return -5;
    }
    if((rs[0] == -3 || rs[0] == -2) && rs[1] == 8 && (rs[2] == -3 || rs[2] == -2)){                    // S -> <exp>|<nnull> relid <exp>|<nnull>
        return -5;
    }
    if((rs[0] == -4 || rs[0] == -2) && rs[1] == 9 && (rs[2] == -4 || rs[2] == -2)){                    // S -> <strexp>|<nnull> relid <strexp>|<nnull>
        return -5;
    }else{
        fprintf(stderr,"redukcia retazca ktory nieje na pravej strane ziadneho pravidla - nespravna syntax");
        exit(2);
    }
}

int expr(token_t* first,token_t* second, token_type end, int skip){

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
    int neterminal;
    int endflag = 0;
    char akcia;
    int b;
    DLList a;
    int q;
    DLL_Init( &a );
    DLL_InsertFirst( &a , 12 );     //vlozenie $ do zasobniku
    DLL_First(&a);
    int rs[3];
    b = get_input(&first,&second,end,skip);
    
    while((b != 12) || (endflag != 1)){
        
        endflag = 0;
        for(int i=0;i<3;i++){
            rs[i] = -1;
        } 
        int i = 0;
        DLL_GetValue(&a,&q);

        akcia = prec_t[q][b];
        if(akcia == '='){
            DLL_InsertLast(&a,b);
            DLL_Last(&a);               //vlozi sa do zoznamu, stane sa aktivnym (kedze to automaticky musi byt najvrchnejsi terminal)
            b = get_input(&first,&second,end,skip);
        }
        else if(akcia == '<'){
            DLL_InsertAfter(&a,'<');        //za aktivny prvok (najvyssi terminal) sa vlozi <
            DLL_InsertLast(&a,b);           //push b
            DLL_Last(&a);                   //make b active (novy najvrchnejsi terminal)
            b = get_input(&first,&second,end,skip);       //precitaj dalsi symbol b zo vstupu
        }
        else if(akcia == '>'){
            DLL_Last(&a);
            DLL_InsertAfter(&a,'>');
            DLL_GetValue(&a,&q);
            while(q != '<'){                //posuvaj sa v zozname dolava pokial nenarazis na <
                DLL_Previous(&a);
                DLL_GetValue(&a,&q);
            }
            DLL_Next(&a);
            DLL_GetValue(&a,&q);
            while(q != '>'){
                rs[i] = q;                //do pola rs sa uklada vyraz medzi <>
                DLL_Next(&a);
                DLL_GetValue(&a,&q);
                i++;
            }

            neterminal = cmp_to_rule(rs);       //v premmenej neterminal je neterminal ktory musime vlozit na zasobnik po tom co odstranime symboly medzi <>, vratane '<' '>' samotnych

            DLL_Last(&a);                       
            while(q != '<'){
                DLL_Previous(&a);
                DLL_DeleteAfter(&a);
                DLL_GetValue(&a,&q);
            }
            DLL_DeleteLast(&a);                 //cely retazec <...> je vymazany
            DLL_InsertLast(&a,neterminal);      //vlozenie neterminalu namiesto <...>
        }
        else if(akcia == 'e'){
            printf("zasobnik = %d,input = %d",q,b);
            fprintf(stderr, "pre kombinaciu zasobnik / vstup nieje v precedencnej tabulke symbol - chybna syntax");
            exit(2);
        }

        DLL_Last(&a);           //kontrola ci zasobnik obsahuje iba $S
        DLL_GetValue(&a, &q);   
        if(q == -5 || q == -3 || q == -4){
            DLL_Previous(&a);
            DLL_GetValue(&a, &q);
            if(q == 12){
                endflag = 1;                //ak ano, nastavi sa endflag na 1
            }
        }

        DLL_Last(&a);
        DLL_GetValue(&a,&q);
        while(q<0 || q == '<' || q == '>' || q == '='){             //od posledneho chod cez vsetky prvky na zasobniku, pokial nenarazis na terminal
            DLL_Previous(&a);
            DLL_GetValue(&a,&q);
        }                                                           //najpravsi terminal je aktivny
    }
    return 0;
}