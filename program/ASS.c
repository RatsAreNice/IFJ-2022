
// Abstraktny Syntakticky Strom
// projekt: IFJ22
/// @authors: Matus Dobias


#include "ASS.h"
#include "Parser.h"
FILE* allahprint; 

void ASSinit(ASSnode_t** tree) { *tree = NULL; }

/// @brief Debugovaci print stromu
/// @param node node k printnutiu
void assprint(ASSnode_t* node) {
   if(node->OP)
  fprintf(allahprint, "[%d,", node->OP);
   else
     printf("[-100,");
  if (node->Patrick_Bateman != NULL)
    fprintf(allahprint, "%s]", node->Patrick_Bateman->value);
  else
    fprintf(allahprint, "-100]");
}

/// @brief Funkcia na tvorba uzlu
/// @param OP Operacia uzlu
/// @param left odkaz na laveho potomka
/// @param right odkaz na laveho potomka
/// @return vracia odkaz na uzol
ASSnode_t* makeTree(operand_t OP, ASSnode_t* left, ASSnode_t* right) {
  ASSnode_t* temp = malloc(sizeof(struct ASSnode));
  if (temp == NULL) {
    exit(99);
  }
  temp->isvar = false;
  temp->leaf = false;
  temp->left = left;
  temp->right = right;
  temp->parent = NULL;
  temp->OP = OP;
  temp->Patrick_Bateman = NULL;

  return temp;
}
/// @brief tvorba listu
/// @param Patrick_Bateman odkaz na token obsiahnuty listom
/// @return vracia odkaz na list
ASSnode_t* makeLeaf(token_t* Patrick_Bateman) {
  ASSnode_t* temp = malloc(sizeof(struct ASSnode));
  token_t* temp2 = malloc(sizeof(struct token));
  if (temp == NULL || temp2 == NULL) {
    exit(99);
  }
  temp->isvar = false;
  temp->leaf = true;
  temp->left = NULL;
  temp->right = NULL;
  temp->parent = NULL;
  temp->OP = LEAF;
  temp2->type = Patrick_Bateman->type;
  temp2->value = Patrick_Bateman->value;
  temp->Patrick_Bateman = temp2;

  return temp;
}
/// @brief free() uzlu
/// @param node mazany uzol
void delete_node(ASSnode_t* node) { free(node); }

/// @brief spustenie rekurzivnej redukcie stromu na kod
/// @param tree koren stromu
void print_code(ASSnode_t** tree) {
  printf(".IFJcode22\n");
  printf("CREATEFRAME\nPUSHFRAME\nCREATEFRAME\n");
  print_builtins();
  helpsolve(*tree);
  if ((*tree)->isvar == true) {
    free(TOK_PATH((*tree))->value);
  }
  printf("EXIT int@0\n");
  free(*tree);
}
/// @brief pomocna funkcia na generaciu kodu
/// @param node uzol premienaný na kód
void ADDInt_Int(ASSnode_t* node) {
  // code for elementary int + int
  LEAFCHECK  // macro

      char* var = createVar();
  printf("DEFVAR LF@%s\n", var);
  GETTHEM
  printf("ADD LF@%s %s %s\n", var, str1, str2);
  FREETHEM
  node->Patrick_Bateman = node->left->Patrick_Bateman;
  node->Patrick_Bateman->type = integer;
  if (node->left->isvar) free(TOK_PATH(node->left)->value);
  if (node->right->isvar) free(TOK_PATH(node->right)->value);
  node->isvar = true;
  node->Patrick_Bateman->value = var;
  node->leaf = true;
  FREESONS
}
/// @brief pomocna funkcia na generaciu kodu
/// @param node uzol premienaný na kód
void ADDFloat_Int(ASSnode_t* node) {
  // code for elementary int + float
  LEAFCHECK  // macro

      char* var = createVar();
  printf("DEFVAR LF@%s\n", var);
  if (node->left->Patrick_Bateman->type == integer) {
    char* tempvar = createVar();
    printf("DEFVAR LF@%s\n", tempvar);
    char* str1 = checkvar(node->left);
    printf("INT2FLOAT LF@%s %s\n", tempvar, str1);
    free(str1);
    node->left->Patrick_Bateman->value = tempvar;
    node->left->Patrick_Bateman->type = ffloat;
  } else {  //
    char* tempvar = createVar();
    printf("DEFVAR LF@%s\n", tempvar);
    char* str1 = checkvar(node->left);
    printf("INT2FLOAT LF@%s %s\n", tempvar, str1);
    free(str1);
    node->right->Patrick_Bateman->value = tempvar;
    node->right->Patrick_Bateman->type = ffloat;
  }
  GETTHEM
  printf("ADD %s %s %s\n", var, str1, str2);
  FREETHEM
  node->Patrick_Bateman = node->left->Patrick_Bateman;
  node->Patrick_Bateman->type = ffloat;
  if (node->left->isvar) free(TOK_PATH(node->left)->value);
  if (node->right->isvar) free(TOK_PATH(node->right)->value);
  node->isvar = true;
  node->Patrick_Bateman->value = var;
  node->leaf = true;
  FREESONS
}
/// @brief pomocna funkcia na generaciu kodu
/// @param node uzol premienaný na kód
void ADDFloat_Float(ASSnode_t* node) {
  // code for elementary float + float
  LEAFCHECK  // macro

      char* var = createVar();
  printf("DEFVAR LF@%s\n", var);
  GETTHEM
  printf("ADD LF@%s %s %s\n", var, str1, str2);
  FREETHEM
  node->Patrick_Bateman = node->left->Patrick_Bateman;
  node->Patrick_Bateman->type = ffloat;
  if (node->left->isvar) free(TOK_PATH(node->left)->value);
  if (node->right->isvar) free(TOK_PATH(node->right)->value);
  node->isvar = true;
  node->Patrick_Bateman->value = var;
  node->leaf = true;
  FREESONS
}
/// @brief pomocna funkcia na generaciu kodu
/// @param node uzol premienaný na kód
void SUBInt_Int(ASSnode_t* node) {
  // code for elementary int - int
  LEAFCHECK  // macro

      char* var = createVar();
  printf("DEFVAR LF@%s\n", var);
  GETTHEM
  printf("SUB LF@%s %s %s\n", var, str1, str2);
  FREETHEM
  node->Patrick_Bateman = node->left->Patrick_Bateman;
  node->Patrick_Bateman->type = integer;
  if (node->left->isvar) free(TOK_PATH(node->left)->value);
  if (node->right->isvar) free(TOK_PATH(node->right)->value);
  node->isvar = true;
  node->Patrick_Bateman->value = var;
  node->leaf = true;
  FREESONS
}
/// @brief pomocna funkcia na generaciu kodu
/// @param node uzol premienaný na kód
void SUBFloat_Int(ASSnode_t* node) {
  // code for elementary int - float
  LEAFCHECK  // macro

      char* var = createVar();
  printf("DEFVAR LF@%s\n", var);
  if (node->left->Patrick_Bateman->type == integer) {
    char* tempvar = createVar();
    printf("DEFVAR LF@%s\n", tempvar);
    char* str1 = checkvar(node->left);
    printf("INT2FLOAT LF@%s %s\n", tempvar, str1);
    free(str1);
    node->left->Patrick_Bateman->value = tempvar;
    node->left->Patrick_Bateman->type = ffloat;
  } else {  //
    char* tempvar = createVar();
    printf("DEFVAR LF@%s\n", tempvar);
    char* str1 = checkvar(node->left);
    printf("INT2FLOAT %s %s\n", tempvar, str1);
    free(str1);
    node->right->Patrick_Bateman->value = tempvar;
    node->right->Patrick_Bateman->type = ffloat;
  }
  GETTHEM
  printf("SUB LF@%s %s %s\n", var, str1, str2);
  FREETHEM
  node->Patrick_Bateman = node->left->Patrick_Bateman;
  node->Patrick_Bateman->type = ffloat;
  if (node->left->isvar) free(TOK_PATH(node->left)->value);
  if (node->right->isvar) free(TOK_PATH(node->right)->value);
  node->isvar = true;
  node->Patrick_Bateman->value = var;
  node->leaf = true;
  FREESONS
}
/// @brief pomocna funkcia na generaciu kodu
/// @param node uzol premienaný na kód
void SUBFloat_Float(ASSnode_t* node) {
  // code for elementary float - float
  LEAFCHECK  // macro

      char* var = createVar();
  printf("DEFVAR LF@%s\n", var);
  GETTHEM
  printf("SUB LF@%s %s %s\n", var, str1, str2);
  FREETHEM
  node->Patrick_Bateman = node->left->Patrick_Bateman;
  node->Patrick_Bateman->type = ffloat;
  if (node->left->isvar) free(TOK_PATH(node->left)->value);
  if (node->right->isvar) free(TOK_PATH(node->right)->value);
  node->isvar = true;
  node->Patrick_Bateman->value = var;
  node->leaf = true;
  FREESONS
}
/// @brief pomocna funkcia na generaciu kodu
/// @param node uzol premienaný na kód
void ASSIGNVAR(ASSnode_t* node) {
  TOK_PATH(node) = TOK_PATH(node->right);
  TOK_PATH(node)->type = TOK_PATH(node->left)->type;
  char* str1 = checkvar(node->left);// TOK_PATH(node->left)->value;
  printf("MOVE LF@%s %s\n",TOK_PATH(node->right)->value,str1);
  node->isvar=true;
  
}
/// @brief pomocna funkcia na generaciu kodu
/// @param node uzol premienaný na kód
void LTCOMP(ASSnode_t* node,bool lolec) {
  char* var1type = createVar();
  char* var2type = createVar();
  char* tmplabel = labelgen();
  char* tmplabel2 = labelgen();
  printf("DEFVAR TF@%s\n",var1type);
  printf("DEFVAR TF@%s\n",var2type);
  GETTHEM
  printf("TYPE TF@%s %s\n",var1type,str1);
  printf("TYPE TF@%s %s\n",var2type,str1);
  printf("JUMPIFEQ %s TF@%s TF@%s\n",tmplabel,var1type,var2type);
  printf("EXIT int@7\n");
  printf("LABEL %s\n",tmplabel);
  char* tempvar = createVar();
  printf("DEFVAR LF@%s\n", tempvar);
  if (lolec)
  {
    printf("EQ LF@%s %s %s",tempvar,str1,str2);
    printf("JUMPIFEQ %s LF@%s bool@true\n",tmplabel2,tempvar);
  }

  printf("LT LF@%s %s %s\n", tempvar, str1, str2);
  printf("LABEL %s\n",tmplabel2);
  FREETHEM
  TOK_PATH(node) = TOK_PATH(node->left);
  TOK_PATH(node)->value = tempvar;
  TOK_PATH(node)->type = bbool;
  node->isvar=true;
  node->leaf = true;
  free(var1type);
  free(var2type);
  free(tmplabel);
  free(tmplabel2);
  FREESONS
}
/// @brief pomocna funkcia na generaciu kodu
/// @param node uzol premienaný na kód
void GTCOMP(ASSnode_t* node,bool lolec) {  // !!! COPYPASTE
   char* var1type = createVar();
  char* var2type = createVar();
  char* tmplabel = labelgen();
  char* tmplabel2 = labelgen();
  printf("DEFVAR TF@%s\n",var1type);
  printf("DEFVAR TF@%s\n",var2type);
  GETTHEM
  printf("TYPE TF@%s %s\n",var1type,str1);
  printf("TYPE TF@%s %s\n",var2type,str1);
  printf("JUMPIFEQ %s TF@%s TF@%s\n",tmplabel,var1type,var2type);
  printf("EXIT int@7\n");
  printf("LABEL %s\n",tmplabel);
  char* tempvar = createVar();
  printf("DEFVAR LF@%s\n", tempvar);
  if (lolec)
  {
    printf("EQ LF@%s %s %s",tempvar,str1,str2);
    printf("JUMPIFEQ %s LF@%s bool@true\n",tmplabel2,tempvar);
  }

  printf("GT LF@%s %s %s\n", tempvar, str1, str2);
  FREETHEM
  TOK_PATH(node) = TOK_PATH(node->left);
  TOK_PATH(node)->value = tempvar;
  TOK_PATH(node)->type = bbool;
  node->isvar = true;
  node->leaf = true;
  free(var1type);
  free(var2type);
  free(tmplabel);
  free(tmplabel2);
  FREESONS
}
/// @brief pomocna funkcia na generaciu kodu
/// @param node uzol premienaný na kód
void EQCOMP(ASSnode_t* node, bool neg) {
  char* tempvar = createVar();
  printf("DEFVAR LF@%s\n", tempvar);
  GETTHEM
  printf("EQ LF@%s %s %s\n", tempvar, str1, str2);
  if (neg)
  {
    printf("NOT LF@%s LF@%s\n",tempvar,tempvar);
  }
  
  FREETHEM
  TOK_PATH(node) = TOK_PATH(node->left);
  TOK_PATH(node)->value = tempvar;
  TOK_PATH(node)->type = bbool;
  node->isvar = true;
  node->leaf = true;
  FREESONS
}
/// @brief pomocna funkcia na generaciu kodu
/// @param node uzol premienaný na kód
void CONCATSTR(ASSnode_t* node) {
  char* tempvar = createVar();
  printf("DEFVAR LF@%s\n", tempvar);
  GETTHEM
  printf("CONCAT LF@%s %s %s\n", tempvar, str1, str2);
  FREETHEM
  if (node->left->isvar) free(TOK_PATH(node->left)->value);
  TOK_PATH(node) = TOK_PATH(node->left);
  TOK_PATH(node)->value = tempvar;
  node->leaf=true;
  node->isvar = true;
}
/// @brief pomocna funkcia na generaciu kodu
/// @param node uzol premienaný na kód
void MULTIPLY(ASSnode_t* node) {
  char* tempvar = createVar();
  printf("DEFVAR LF@%s\n", tempvar);
  GETTHEM
  printf("MUL LF@%s %s %s\n", tempvar, str1, str2);
  FREETHEM
  if (node->left->isvar) free(TOK_PATH(node->left)->value);
  TOK_PATH(node) = TOK_PATH(node->left);
  TOK_PATH(node)->value = tempvar;
  node->leaf=true;
  node->isvar = true;
}
void DIVIS(ASSnode_t* node) {
  char* tempvar = createVar();
  printf("DEFVAR LF@%s\n", tempvar);
  GETTHEM
  printf("DIV LF@%s %s %s\n", tempvar, str1, str2);
  FREETHEM
  if (node->left->isvar) free(TOK_PATH(node->left)->value);
  TOK_PATH(node) = TOK_PATH(node->left);
  TOK_PATH(node)->value = tempvar;
  node->leaf=true;
  node->isvar = true;
}
/// @brief pomocna funkcia na generaciu kodu
/// @param node uzol premienaný na kód
void generateif(ASSnode_t* node) {
if (node->left->leaf==false)
{
  helpsolve(node->left);
}
static unsigned int ifcount;

printf("\nDEFVAR TF@var%d\n",++ifcount);
printf("DEFVAR TF@JMPCOND%d\n",ifcount);
printf("DEFVAR TF@VARTYPE%d\n",ifcount);
// generate expression


printf("MOVE TF@var%d %s\n",ifcount,checkvar(node->left));


printf("TYPE TF@VARTYPE%d TF@var%d\n",ifcount,ifcount);
printf("JUMPIFNEQ %%skipboolcheck%d TF@VARTYPE%d string@bool\n",ifcount,ifcount);
printf("MOVE TF@JMPCOND%d TF@var%d\n",ifcount,ifcount);
printf("JUMP %%IFBODY%d\n",ifcount);
printf("LABEL %%skipboolcheck%d\n",ifcount);
printf("JUMPIFNEQ %%skipstrcheck%d TF@VARTYPE%d string@string\n",ifcount,ifcount);
printf("JUMPIFEQ %%ELSE%d TF@var%d string@\n",ifcount,ifcount);
printf("MOVE TF@JMPCOND%d bool@true\n",ifcount);
printf("JUMP %%IFBODY%d\n",ifcount);
printf("LABEL %%skipstrcheck%d\n",ifcount);
printf("JUMPIFNEQ %%skipintcheck%d TF@VARTYPE%d string@int\n",ifcount,ifcount);
printf("JUMPIFEQ %%ELSE%d TF@var%d int@0\n",ifcount,ifcount);
printf("MOVE TF@JMPCOND%d bool@true\n",ifcount);
printf("JUMP %%IFBODY%d\n",ifcount);
printf("LABEL %%skipintcheck%d\n",ifcount);
printf("JUMPIFNEQ %%skipfloatcheck%d TF@VARTYPE%d string@float\n",ifcount,ifcount);
printf("JUMPIFEQ %%ELSE%d TF@var%d float@0x0p+0\n",ifcount,ifcount);
printf("MOVE TF@JMPCOND%d bool@true\n",ifcount);
printf("JUMP %%IFBODY%d\n",ifcount);
printf("LABEL %%skipfloatcheck%d\n",ifcount);
printf("MOVE TF@JMPCOND%d bool@false\n",ifcount);
printf("JUMP %%IFBODY%d\n",ifcount);

printf("LABEL %%IFBODY%d\n",ifcount);
printf("JUMPIFEQ %%IFTHEN%d TF@JMPCOND%d bool@true\n",ifcount,ifcount);
printf("LABEL %%ELSE%d\n",ifcount);
//else
if (node->right->right!=NULL)
{
  helpsolve(node->right->right);
}
printf("JUMP %%ENDIF%d\n",ifcount);
printf("LABEL %%IFTHEN%d\n",ifcount);
//then
if (node->right->right!=NULL)
{
  helpsolve(node->right->left);
}
printf("LABEL %%ENDIF%d\n",ifcount);
node->right->leaf=true;
node->leaf=true;
}
/// @brief pomocna funkcia na generaciu kodu
/// @param node uzol premienaný na kód
void generatedec(ASSnode_t* node){
  static unsigned int deccount;
  static unsigned int pcount;
  char* fid = TOK_PATH(node->left->left->left)->value;
  ASSnode_t* nparam;

  printf("JUMP %%skipdec%d\n",++deccount);
  // BODY OF FUN
  printf("LABEL %s\n",fid);
  while (node->left->right->left!=NULL)
  {
    nparam = node->left->right->left;
    printf("DEFVAR LF@%s",TOK_PATH(nparam->right)->value);
    printf("MOVE LF@%s TF@param%d\n",TOK_PATH(nparam->right)->value,pcount);
    printf("DEFVAR TF@%%param%dtype\n",pcount);
    printf("TYPE TF@%%param%dtype TF@param%d\n",pcount,pcount);
    printf("JUMPIFNEQ %%%sEXIT4 TF@%%param%dtype string@%s\n",fid,pcount,TOK_PATH(nparam->left)->value);
    node->left->right=node->left->right->right;
    if (node->left->right==NULL) break;
    pcount++;
  }
  pcount=0;
  
  helpsolve(node->right);
  
  printf("DEFVAR TF@%%rettype\n");
  printf("TYPE TF@%%rettype LF@%%ret%s\n",fid);
  printf("JUMPIFNEQ %%%sEXIT4 TF@%%rettype string@%s\n",fid,TOK_PATH(node->left->left->right)->value);
  printf("RETURN\n");
  printf("LABEL %%%sEXIT4\n",fid);
  printf("EXIT int@4\n");
  printf("LABEL %%skipdec%d\n",deccount);


}
/// @brief hlavna funkcia riadiaca generaciu kodu
/// @param node rieseny uzol
void helpsolve(ASSnode_t* node) {
  switch (node->OP) {
    case ADD:
      LEAFCHECK  // macro
          if (node->left->leaf == true && node->right->leaf == true) {
        if (node->left->Patrick_Bateman->type == ffloat &&
            node->right->Patrick_Bateman->type == ffloat) {
          ADDFloat_Float(node);
        } else if (node->left->Patrick_Bateman->type == integer &&
                   node->right->Patrick_Bateman->type == integer) {
          ADDInt_Int(node);
        } else {
          ADDFloat_Int(node);
        }
        break;
      }

      break;

    case SUB:
      LEAFCHECK  // macro
          if (node->left->leaf == true && node->right->leaf == true) {
        if (node->TOK_PATH(left)->type == ffloat &&
            node->TOK_PATH(right)->type == ffloat) {
          SUBFloat_Float(node);
        } else if (node->TOK_PATH(left)->type == integer &&
                   node->TOK_PATH(right)->type == integer) {
          SUBInt_Int(node);
        } else {
          SUBFloat_Int(node);
        }
        break;
      }
      else {
        fprintf(stderr, "SOMETHING WEIRD HAPPENED IN SUB\n");
        exit(7);
      }
      break;
    case MUL:
      LEAFCHECK
      if (node->left->leaf == true && node->right->leaf == true) {
        if (node->TOK_PATH(left)->type == node->TOK_PATH(right)->type) {
         
          MULTIPLY(node);

        } else if ((node->TOK_PATH(left)->type == integer &&
                   node->TOK_PATH(right)->type == ffloat) ||
                   (node->TOK_PATH(right)->type == integer &&
                   node->TOK_PATH(left)->type == ffloat) 
                   ) {
          if (node->TOK_PATH(left)->type == integer)
          {
            char* str1=checkvar(node->left);
            if (node->left->isvar==true) 
            {
              printf("INT2FLOAT %s %s\n",str1,str1);
              TOK_PATH(node->left)->type=ffloat;
            }else
            {
              char* tmp = createVar();
              printf("DEFVAR LF@%s\n",tmp);
              printf("INT2FLOAT LF@%s %s\n",tmp,str1);
              node->left->isvar=true;
              TOK_PATH(node->left)->type=ffloat;
              TOK_PATH(node->left)->value=tmp;
            }
            node->TOK_PATH(left)->type = ffloat;
            free(str1);
            MULTIPLY(node);
          } else
          {
            char* str1=checkvar(node->right);
            if (node->left->isvar==true) 
            {
              printf("INT2FLOAT %s %s\n",str1,str1);
              TOK_PATH(node->right)->type=ffloat;
            }else
            {
              char* tmp = createVar();
              printf("DEFVAR LF@%s\n",tmp);
              printf("INT2FLOAT LF@%s %s\n",tmp,str1);
              node->left->isvar=true;
              TOK_PATH(node->right)->type=ffloat;
              TOK_PATH(node->right)->value=tmp;
            }
            node->TOK_PATH(right)->type = ffloat;
            free(str1);
            MULTIPLY(node);
          }
          

        }else{ exit(7);} 

        break;
      }
      else {
        fprintf(stderr, "SOMETHING WEIRD HAPPENED IN MUL\n");
        exit(7);
      }
      break;
    break;
    case ASSIGN:
      if (node->left->leaf == false) helpsolve(node->left);
      ASSIGNVAR(node);

        /*if (TOK_PATH(node->left)->type == TOK_PATH(node)->type) {
          ASSIGNVAR(node);
          break;
        } else if (TOK_PATH(node)->type == ffloat &&
                   TOK_PATH(node->left)->type == integer) {
          ASSIGNVARFloat_Int(node);
          break;
        } else if (TOK_PATH(node)->type == ffloat &&
                   TOK_PATH(node->left)->type == integer) {
          ASSIGNVARFloat_Int(node);
          break;
        }
      } else {
        fprintf(stderr, "SOMETHING WEIRD HAPPENED IN ASSIGN\n");
        exit(7);
      }*/
      break;
    // case EQ:
    case GT:
    case LT:
    case LTE:
    case GTE:
      LEAFCHECK  // macro
      char* tstr1 = CHECKTYPE(node->left);
      char* tstr2 = CHECKTYPE(node->right);
      GETTHEM
      printf("DEFVAR %s\n", tstr1);
      printf("DEFVAR %s\n", tstr2);
      printf("TYPE %s %s\n", tstr1, str1);
      printf("TYPE %s %s\n", tstr2, str2);
      char* label = labelgen();
      char* label2 = labelgen();
      printf("JUMPIFEQ %s %s string@nil\n", label, tstr1);
      printf("JUMPIFEQ %s %s string@nil\n", label, tstr2);
      printf("JUMP %s\n", label2);
      printf("LABEL %s\n", label);
      printf("EXIT int@7\n");
      printf("LABEL %s\n", label2);
      free(label);
      free(label2);

      if (node->left->leaf == true && node->right->leaf == true) {
        if (TOK_PATH(node->left)->type == TOK_PATH(node->right)->type) {
          switch (node->OP)
          {
          case LT:
            LTCOMP(node,false);
            break;
          case GT:
            GTCOMP(node,false);
            break;
          case LTE:
            LTCOMP(node,true);
            break;
          case GTE:
            GTCOMP(node,true);
            break;
            default:
            break;
          }

        } else if (TOK_PATH(node->left)->type == string &&
                   !(TOK_PATH(node->right)->type == string)) {
          fprintf(stderr, "Expected a string in the 2nd operand");
          exit(7);
        } else if ((TOK_PATH(node->left)->type == integer ||
                    TOK_PATH(node->left)->type == ffloat) &&
                   (TOK_PATH(node->right)->type == integer ||
                    TOK_PATH(node->right)->type == ffloat)) {
          // conversion
          char* tempvar = createVar();
          if (TOK_PATH(node->left)->type == integer) {
            printf("DEFVAR LF@%s\n", tempvar);
            printf("INT2FLOAT LF@%s %s\n", tempvar, str1);
            TOK_PATH(node->left)->type = ffloat;
            TOK_PATH(node->left)->value = tempvar;
            node->right->isvar = true;

          } else {
            printf("DEFVAR LF@%s\n", tempvar);
            printf("INT2FLOAT LF@%s  %s\n", tempvar, str2);
            TOK_PATH(node->right)->type = ffloat;
            TOK_PATH(node->right)->value = tempvar;
            node->right->isvar = true;
            // do cmp
          }
          switch (node->OP)
          {
          case LT:
            LTCOMP(node,false);
            break;
          case GT:
            GTCOMP(node,false);
            break;
          case LTE:
            LTCOMP(node,true);
            break;
          case GTE:
            GTCOMP(node,true);
            break;
          default:
          break;
          }
        }
      } else {
        fprintf(stderr, "SOMETHING WEIRD HAPPENED IN GT/LT\n");
        exit(7);
      }
      break;
    case NEQ:
    case EQ:
      LEAFCHECK
      if (node->left->leaf == true && node->right->leaf == true) {
        if (TOK_PATH(node->left)->type == TOK_PATH(node->right)->type) {
          if (node->OP==EQ)
          {
            EQCOMP(node,false);
          }else{
            EQCOMP(node,true);
          }
          
          
          
        } else if (TOK_PATH(node->left)->type == string &&
                   !(TOK_PATH(node->right)->type == string)) {
          fprintf(stderr, "Expected a string in the 2nd operand");
          exit(7);
        } else if ((TOK_PATH(node->left)->type == integer ||
                    TOK_PATH(node->left)->type == ffloat) &&
                   (TOK_PATH(node->right)->type == integer ||
                    TOK_PATH(node->right)->type == ffloat)) {
          // conversion
          GETTHEM
          char* tempvar = createVar();
          if (TOK_PATH(node->left)->type == integer) {
            printf("DEFVAR LF@%s\n", tempvar);
            printf("INT2FLOAT LF@%s %s ", tempvar, str1);
            TOK_PATH(node->left)->type = ffloat;
            TOK_PATH(node->left)->value = tempvar;
            node->right->isvar = true;

          } else {
            printf("DEFVAR LF@%s\n", tempvar);
            printf("INT2FLOAT LF@%s  %s", tempvar, str2);
            TOK_PATH(node->right)->type = ffloat;
            TOK_PATH(node->right)->value = tempvar;
            node->right->isvar = true;
            // do cmp
          }
          if (node->OP==EQ)
          {
            EQCOMP(node,false);
          }else{
            EQCOMP(node,true);
          }
          FREETHEM
        }else{
          char* var1type = createVar();
          char* var2type = createVar();
          char* tmplabel = labelgen();
          printf("DEFVAR TF@%s\n",var1type);
          printf("DEFVAR TF@%s\n",var2type);
          GETTHEM
          printf("TYPE TF@%s %s\n",var1type,str1);
          printf("TYPE TF@%s %s\n",var2type,str1);
          printf("JUMPIFEQ %s TF@%s TF@%s\n",tmplabel,var1type,var2type);
          printf("EXIT int@7\n");
          printf("LABEL %s\n",tmplabel);
          if (node->OP==EQ)
          {
            EQCOMP(node,false);
          }else{
            EQCOMP(node,true);
          }
          FREETHEM
          free(var1type);
          free(var2type);
          free(tmplabel);
        }
      } else {
        fprintf(stderr, "SOMETHING WEIRD HAPPENED IN EQ\n");
       // exit(7);
      }
      break;
    
    case CONCAT:
      LEAFCHECK
      if ((TOK_PATH(node->left)->type != string && node->left->isvar==false && TOK_PATH(node->left)->type != ID_variable)||(
          TOK_PATH(node->right)->type != string && node->right->isvar==false && TOK_PATH(node->right)->type != ID_variable)) {
        
        fprintf(stderr, "CONCATENATION OF NON-STRING OPERANDS %d %d\n",TOK_PATH(node->left)->type,TOK_PATH(node->right)->type);
       // exit(7);
      }
      CONCATSTR(node);
      break;
    case IF:
      if (node->left->leaf==false) helpsolve(node->left);
      generateif(node);
      break;
    case FUNCTIONCALL:
      if (strcmp(TOK_PATH(node->right)->value,"write")==0)
      { 
        while (node->left!=NULL)
        {
       // fprintf(stderr,"ENTERING WRITE\n");
        char* tmp = checkvar(node->left->left);
       // fprintf(stderr,"LEAVING WRITE\n");
        printf("WRITE %s\n",tmp);
        free(tmp);
        node->left=node->left->right;
        }
      }else
      {
        node->Patrick_Bateman=node->right->Patrick_Bateman;
        TOK_PATH(node)->type=bst_search(symDLL_GetFirst(&symtablelist),TOK_PATH(node->right)->value)->funData->returnType;
        /*fprintf(stderr,"%s",TOK_PATH(node->right)->value);
        fprintf(stderr,"%d",TOK_PATH(node)->type);*/
        /*
        
        BLOCK FOR FUNCTIONCALLING
        
        */
        printf("CREATEFRAME\n");
        for (int i = 0; i < bst_search(symDLL_GetFirst(&symtablelist),TOK_PATH(node->right)->value)->funData->ParamCount; i++)
        {
          printf("DEFVAR TF@param%d\n",i);
          printf("MOVE TF@param%d %s\n",i,checkvar(node->left->left));
          node->left=node->left->right;
          if (node->left == NULL) break;
        }
        printf("CALL %s\n",TOK_PATH(node->right)->value);
        node->isvar=true;
        char* trollec= malloc(sizeof(char)*(6+strlen(TOK_PATH(node->right)->value)));
        sprintf(trollec,"%%ret%s\n",TOK_PATH(node->right)->value);
        TOK_PATH(node)->value = trollec;
        node->leaf=true;
        LEAFCHECK
      }
      break;
    case ARGS:
      LEAFCHECK
      break; 
    case DIV:
      LEAFCHECK
      if (node->left->leaf == true && node->right->leaf == true) {
        if (node->TOK_PATH(left)->type == node->TOK_PATH(right)->type) {
          DIVIS(node);
          TOK_PATH(node)->type = ffloat;
        } else if ((node->TOK_PATH(left)->type == integer &&
                   node->TOK_PATH(right)->type == ffloat) ||
                   (node->TOK_PATH(right)->type == integer &&
                   node->TOK_PATH(left)->type == ffloat) 
                   ) {
          if (node->TOK_PATH(left)->type == integer)
          {
            char* str1=checkvar(node->left);
            if (node->left->isvar==true) 
            {
              printf("INT2FLOAT %s %s\n",str1,str1);
              TOK_PATH(node->left)->type=ffloat;
            }else
            {
              char* tmp = createVar();
              printf("DEFVAR LF@%s\n",tmp);
              printf("INT2FLOAT LF@%s %s\n",tmp,str1);
              node->left->isvar=true;
              TOK_PATH(node->left)->type=ffloat;
              TOK_PATH(node->left)->value=tmp;
            }
            node->TOK_PATH(left)->type = ffloat;
            free(str1);
            DIVIS(node);
            TOK_PATH(node)->type = ffloat;
          } else
          {
            char* str1=checkvar(node->right);
            if (node->left->isvar==true) 
            {
              printf("INT2FLOAT %s %s\n",str1,str1);
              TOK_PATH(node->right)->type=ffloat;
            }else
            {
              char* tmp = createVar();
              printf("DEFVAR LF@%s\n",tmp);
              printf("INT2FLOAT LF@%s %s\n",tmp,str1);
              node->left->isvar=true;
              TOK_PATH(node->right)->type=ffloat;
              TOK_PATH(node->right)->value=tmp;
            }
            node->TOK_PATH(right)->type = ffloat;
            free(str1);
            DIVIS(node);
            TOK_PATH(node)->type = ffloat;
          }
          

        }else{ exit(7);} 

        break;
      }
      else {
        fprintf(stderr, "SOMETHING WEIRD HAPPENED IN DIV\n");
        exit(7);
      }
      break;

      break;
    case FDEC:
      generatedec(node);
      break;   
    case RYAN_GOSLING:
      LEAFCHECK
      break;
    default:
      /*fprintf(stderr, "UNEXPECTED OPERAND (%d) HOW DID THAT HAPPEN\n",
              node->OP);
      exit(7);*/
      LEAFCHECK
      break;
  }
}

/// @brief pomocna funkcia na prepisanie floatu na hex reprezentaciu
/// @param Patrick_Bateman token obsahujuci float
void floatify(token_t* Patrick_Bateman) {
  sprintf(Patrick_Bateman->value, "%a", strtof(Patrick_Bateman->value, NULL));
}
/// @brief pomocna funkcia na vypis premennych v IFJcode22
/// @param node uzol obsahujuci premennu
/// @return vracia formatovany string pre pracu s premennou
char* CHECKTYPE(ASSnode_t* node) {
  static unsigned int typevarcount;  // LF@tempvar + uint + \0
  char* str = malloc(sizeof(char) * 21);
  if (str == NULL) exit(99);
  sprintf(str, "LF@tempvar%d", typevarcount);
  typevarcount++;
  return str;
}
/// @brief pomocna funkcia na generaciu unikatneho navestia
/// @return vracia string s unikatnym navestim
char* labelgen() {
  static unsigned int labelcount;
  char* str = malloc(sizeof(char) * 16);  // label + uint + \0
  if (str == NULL) exit(99);
  sprintf(str, "label%d", labelcount);
  labelcount++;
  return str;
}
/// @brief pomocna funkcia pre pracu s terminalom v IFJcode22
/// @param node uzol obsahujuci premennu/konstantu
/// @return vracia formatovany string pre pracu s premennou alebo konstantou 
char* checkvar(ASSnode_t* node) {
  if (TOK_PATH(node)==NULL)
  {
    fprintf(stderr,"co?\n");
    exit(1);
  }else if (TOK_PATH(node)->value==NULL)
  {
    fprintf(stderr,"STO?!\n");
  }
  char* strptr = malloc(sizeof(char)*(22+strlen(TOK_PATH(node)->value)));  // 
  if (strptr == NULL) exit(99);

  
  
  
  if (TOK_PATH(node)->type==ID_variable)
  {
    sprintf(strptr, "LF@%s", TOK_PATH(node)->value);
    return strptr;
  }
  if (node->isvar) {
    sprintf(strptr, "LF@%s", TOK_PATH(node)->value);
    return strptr;
  } else {
    switch (TOK_PATH(node)->type) {
      case integer:
        sprintf(strptr, "int@%s", TOK_PATH(node)->value);
        return strptr;
      case ffloat:
        floatify(TOK_PATH(node));
        sprintf(strptr, "float@%s", TOK_PATH(node)->value);
        return strptr;
      case string:
        sprintf(strptr, "string@%s", TOK_PATH(node)->value);
        return strptr;
      case funnull:
        sprintf(strptr, "nil@nil");
        return strptr;
      case bbool:
        sprintf(strptr, "LF@%s", TOK_PATH(node)->value);
        return strptr;

      default:
        fprintf(stderr, "JIHAD JIHAD JIHAD %d\n",TOK_PATH(node)->type);
        exit(17);
    }
  }
}
/// @brief funkcia pre generovanie unikatnej pomocnej premennej
/// @return formatovany string s unikatnou premennou
char* createVar() {
  static unsigned int varcount;
  char* str = malloc(sizeof(char) * 14);  // var + maxuint length(10) + \0 asi
  if (str == NULL) exit(99);
  sprintf(str, "var%d", varcount);
  varcount++;
  return str;
}
void reads() {
  printf("LABEL reads\n");
  printf("READ LF@%%retreads string\n");
  printf("RETURN\n");
}
void readi() {
  printf("LABEL readi\n");
  printf("READ LF@%%retreadi int\n");
  printf("RETURN\n");
}
void readf() {
  printf("LABEL readf\n");
  printf("READ LF@%%retreadf float\n");
  printf("RETURN\n");
}
void phpsubstring() {
  printf("LABEL substring");
  printf("\nMOVE LF@%%retsubstring nil@nil\nDEFVAR TF@lolec");
  printf("\nSTRLEN TF@lolec TF@param1");
  printf("\nDEFVAR TF@zerocheck");
  printf("\nDEFVAR TF@zerocheck2");
  printf("\nDEFVAR TF@ltcheck");
  printf("\nDEFVAR TF@lencheck1");
  printf("\nDEFVAR TF@lencheck2");
  printf("\nDEFVAR TF@loopcheck");
  printf("\nDEFVAR TF@i");
  printf("\nDEFVAR TF@L1b");
  printf("\nLT TF@zerocheck TF@param2 int@0");
  printf("\nLT TF@zerocheck2 TF@param3 int@0");
  printf("\nLT TF@ltcheck TF@param3 TF@param2");
  printf("\nLT TF@lencheck1 TF@lolec TF@param2");
  printf("\nGT TF@lencheck2 TF@param3 TF@lolec");
  printf("\nJUMPIFEQ %%substrfail bool@true TF@zerocheck");
  printf("\nJUMPIFEQ %%substrfail bool@true TF@zerocheck2");
  printf("\nJUMPIFEQ %%substrfail bool@true TF@ltcheck");
  printf("\nJUMPIFEQ %%substrfail bool@true TF@lencheck1");
  printf("\nJUMPIFEQ %%substrfail bool@true TF@lencheck2");
  printf("\nJUMPIFEQ %%substrdone TF@param2 TF@param3");
  printf("\nGETCHAR LF@%%retsubstring TF@param1 TF@param2");
  printf("\nADD TF@param2 TF@param2 int@1");
  printf("\nJUMPIFEQ %%substrdone TF@param2 TF@param3");
  printf("\nLABEL %%substrL1");
  printf("\nGETCHAR TF@i TF@param1 TF@param2");
  printf("\nCONCAT LF@%%retsubstring LF@%%retsubstring TF@i");
  printf("\nADD TF@param2 TF@param2 int@1");
  printf("\nLT TF@loopcheck TF@param2 TF@param3");
  printf("\nJUMPIFNEQ %%substrL1 TF@loopcheck bool@false");
  printf("\nLABEL %%substrdone");
  printf("\nCREATEFRAME");
  printf("\nRETURN");
  printf("\nLABEL %%substrfail");
  printf("\nCREATEFRAME\nRETURN\n");
}
void phpstrlen() {
  printf("LABEL %%strlen\n");
  printf("DEFVAR TF@retval\n");
  printf("STRLEN TF@retval TF@param1\n");
  printf("MOVE LF@%%retstrlen TF@retval\n");
  printf("CREATEFRAME\nRETURN\n");
}
void phpfloatval() {
  printf("\nLABEL floatval");
  printf("\nDEFVAR TF@TYPESTR");
  printf("\nTYPE TF@TYPESTR TF@param1");
  printf("\nJUMPIFEQ %%fvalfail TF@TYPESTR string@string");
  printf("\nJUMPIFEQ %%fvalbool TF@TYPESTR string@bool");
  printf("\nJUMPIFEQ %%fvalnil TF@TYPESTR string@nil");
  printf("\nJUMPIFEQ %%fvalfloat TF@TYPESTR string@float");
  printf("\nINT2FLOAT TF@param1 TF@param1");
  printf("\nLABEL %%fvalfloat");
  printf("\nMOVE LF@%%retfloatval TF@param1");
  printf("\nCREATEFRAME");
  printf("\nRETURN");
  printf("\nLABEL %%fvalfail");
  printf("\nEXIT int@7");
  printf("\nLABEL %%fvalbool");
  printf("\nMOVE LF@%%retfloatval float@0x0p+0");
  printf("\nJUMPIFEQ %%fvalfalse TF@param1 bool@false");
  printf("\nMOVE LF@%%retfloatval float@0x1p+0");
  printf("\nLABEL %%fvalfalse");
  printf("\nCREATEFRAME");
  printf("\nRETURN");
  printf("\nLABEL %%fvalnil");
  printf("\nMOVE LF@%%retfloatval float@0x0p+0");
  printf("\nCREATEFRAME");
  printf("\nRETURN\n");
}
void phpintval() {
  printf("\nLABEL intval");
  printf("\nDEFVAR TF@TYPESTR");
  printf("\nTYPE TF@TYPESTR TF@param1");
  printf("\nJUMPIFEQ %%ivalfail TF@TYPESTR string@string");
  printf("\nJUMPIFEQ %%ivalbool TF@TYPESTR string@bool");
  printf("\nJUMPIFEQ %%ivalnil TF@TYPESTR string@nil");
  printf("\nJUMPIFEQ %%ivalint TF@TYPESTR string@int");
  printf("\nFLOAT2INT TF@param1 TF@param1");
  printf("\nLABEL %%ivalint");
  printf("\nMOVE LF@%%retintval TF@param1");
  printf("\nCREATEFRAME");
  printf("\nRETURN");
  printf("\nLABEL %%ivalfail");
  printf("\nDPRINT string@LOLEC");
  printf("\nEXIT int@7");
  printf("\nLABEL %%ivalbool");
  printf("\nMOVE LF@%%retintval int@0");
  printf("\nJUMPIFEQ %%ivalfalse TF@param1 bool@false ");
  printf("\nMOVE LF@%%retintval int@1");
  printf("\nLABEL %%ivalfalse");
  printf("\nCREATEFRAME");
  printf("\nRETURN");
  printf("\nLABEL %%ivalnil");
  printf("\nMOVE LF@%%retintval int@0");
  printf("\nCREATEFRAME");
  printf("\nRETURN\n");
}

/// @brief Preorder priechod tabulkou symbolov pre deklaraciu premennych v maine
/// @param tree odkaz na tabulku symbolov
void PreOrder(bst_node_t* tree){
  if (tree==NULL)
  {
    return;
  }
  
  if (tree->type==16)
  {
    printf("DEFVAR LF@%s\n",tree->key);
  }else
  {
    printf("DEFVAR LF@%%ret%s\n",tree->key);
  }
  PreOrder(tree->left);
  PreOrder(tree->right);
}

/// @brief funkcia na tlacenie vstavanych funkcii a premennych
void print_builtins() {
  PreOrder(symDLL_GetFirst(&symtablelist));
  /*printf("DEFVAR LF@%%retintval\n");
  printf("DEFVAR LF@%%retfloatval\n");
  printf("DEFVAR LF@%%retsubstring\n");
  printf("DEFVAR LF@%%retreads\n");
  printf("DEFVAR LF@%%retreadi\n");
  printf("DEFVAR LF@%%retreadf\n");
  printf("DEFVAR LF@%%retstrlen\n");*/
 
  printf("JUMP main\n");
  reads();
  readi();
  readf();
  phpsubstring();
  phpstrlen();
  phpfloatval();
  phpintval();
  printf("LABEL main\n");
}