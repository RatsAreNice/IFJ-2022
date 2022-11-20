#include "../Source/symtable.c"

int main (){
    bst_node_t* root;
    bst_init(&root);

    bool trollec[11]={true,false,false,false,true,true,false,false,false,false,false};
    int pCount[11]= {5,2,1,1,1,0,0,1,4,2,7};
    int gay0[5] = {INT,STRING,STRING,INT,STRING};
    int gay1[2] = {INT,INT};
    int gay2[1] = {INT};
    int gay3[1] = {INT};
    int gay4[1] = {INT};
    //int gay5[1];// = NULL;
    //int gay6[1];// = NULL;
    int gay7[1] = {INT};
    int gay8[4] = {INT,STRING,STRING,STRING};
    int gay10[7] = {INT,STRING,STRING,INT,STRING,INT,INT};
    int gay9[2] = {STRING,STRING};
    int* pTypes[11]= {gay0,gay1,gay2,gay3,gay4,NULL,NULL,gay7,gay8,gay9,gay10};
    int rTypes[11]= {INT,STRING,VOID,INT,INT,VOID,STRING,INT,VOID,INT,STRING};
    char* trolls[11]={"6kokocina\0","2kokocina\0","3kokocina\0","1kokocina\0","0kokocina\0","4kokocina\0","5kokocina\0","7kokocina\0","8kokocina\0","9kokocina\0","99kokocina\0"};
    
    for (size_t i = 0; i < 11; i++)
    {
        funData_t* XD = malloc(sizeof(struct funData));
        XD->defined=trollec[i];
        XD->ParamCount=pCount[i];
        XD->paramTypes=pTypes[i];
        XD->returnType=rTypes[i];
        bst_insert(&root,trolls[i],1,XD);
    }
    bst_node_t* temp = bst_search(root,"3kokocina\0");
    printf(" 3kokocina:\n defined: %d\n pCount: %d\n returnType: %d\n key: %s\n",temp->funData->defined,temp->funData->ParamCount,temp->funData->returnType,temp->key);
    temp->funData->returnType=127;
    temp = bst_search(root,"3kokocina\0");
    printf(" returnType: %d\n",temp->funData->returnType);
    bst_dispose(&root);
    bst_delete(&root,"3kokocina\0");




    return 0;
}