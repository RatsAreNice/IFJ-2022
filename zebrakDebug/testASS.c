#include "../Source/ASS.h"

int main(){

    // generate code for 5+7-3-4+2

    ASSnode_t** root;

    token_t five;
    five.type=integer;
    five.value="5";
    token_t seven;
    seven.type=integer;
    seven.value="7";
    token_t three;
    three.type=integer;
    three.value="3";
    token_t four;
    four.type=integer;
    four.value="4";
    token_t two;
    five.type=integer;
    five.value="2";

    ASSnode_t* fiveleaf = makeLeaf(&five);
    ASSnode_t* sevenleaf = makeLeaf(&three);
    ASSnode_t* threeleaf = makeLeaf(&seven);
    ASSnode_t* fourleaf = makeLeaf(&four);
    ASSnode_t* twoleaf = makeLeaf(&two);

    ASSnode_t* trollec = makeTree(ADD,fiveleaf,sevenleaf);
    printf("%s\n",five.value);
    /*ASSnode_t* lolec   = makeTree(SUB,trollec,threeleaf);
    ASSnode_t* holec   = makeTree(SUB,lolec,fourleaf);
    ASSnode_t* molec   = makeTree(ADD,holec,twoleaf);
    root=&molec;*/

    root=&trollec;
    print_code(root);

    return 0;
}