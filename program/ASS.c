// Abstraktny Syntakticky Strom
// autor: Matus Dobias

#include "ASS.h"
FILE* allahprint;

void ASSinit(ASSnode_t** tree) { *tree = NULL; }

void assprint(ASSnode_t* node) {
  // if(node->OP)
  fprintf(allahprint, "[%d,", node->OP);
  // else
  //   printf("[-100,");
  if (node->Patrick_Bateman != NULL)
    fprintf(allahprint, "%s]", node->Patrick_Bateman->value);
  else
    fprintf(allahprint, "-100]");
}

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

void delete_node(ASSnode_t* node) { free(node); }

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

void ASSIGNVAR(ASSnode_t* node) {
  char* var = malloc(sizeof(char) * strlen(TOK_PATH(node)->value));

  strcpy(var, TOK_PATH(node)->value);
  TOK_PATH(node)->value = var;  // CHCEM SA UISTIT ZE TAM BUDE MALLOC

  printf("DEFVAR LF@%s\n", TOK_PATH(node)->value);
  TOK_PATH(node)->type = TOK_PATH(node->left)->type;
  node->leaf = true;
  node->isvar = true;
  char* str1 = checkvar(node);
  char* str2 = checkvar(node->left);
  printf("MOVE %s %s\n", str1, str2);
  FREETHEM
  delete_node(node->left);
}

void ASSIGNVARInt_Float(ASSnode_t* node) {
  char* tempvar = createVar();
  printf("DEFVAR LF@%s\n", tempvar);
  char* str1 = checkvar(node->left);
  printf("FLOAT2INT LF@%s %s\n", tempvar, str1);
  free(str1);
  if (node->left->isvar) free(TOK_PATH(node->left)->value);
  TOK_PATH(node->left)->value = tempvar;
  TOK_PATH(node->left)->type = integer;
  ASSIGNVAR(node);
}
void ASSIGNVARFloat_Int(ASSnode_t* node) {
  char* tempvar = createVar();
  printf("DEFVAR LF@%s\n", tempvar);
  char* str1 = checkvar(node->left);
  printf("INT2FLOAT LF@%s %s\n", tempvar, str1);
  free(str1);
  if (node->left->isvar) free(TOK_PATH(node->left)->value);
  TOK_PATH(node->left)->value = tempvar;
  TOK_PATH(node->left)->type = integer;
  ASSIGNVAR(node);
}
void LTCOMP(ASSnode_t* node) {
  char* tempvar = createVar();
  printf("DEFVAR LF@%s\n", tempvar);
  GETTHEM
  printf("LT LF@%s %s %s\n", tempvar, str1, str2);
  FREETHEM
  TOK_PATH(node) = TOK_PATH(node->left);
  TOK_PATH(node)->value = tempvar;
  TOK_PATH(node)->type = bbool;
  FREESONS
}
void GTCOMP(ASSnode_t* node) {  // !!! COPYPASTE
  char* tempvar = createVar();
  printf("DEFVAR LF@%s\n", tempvar);
  GETTHEM
  printf("LT LF@%s %s %s\n", tempvar, str1, str2);
  FREETHEM
  TOK_PATH(node) = TOK_PATH(node->left);
  TOK_PATH(node)->value = tempvar;
  TOK_PATH(node)->type = bbool;
  FREESONS
}
void EQCOMP(ASSnode_t* node) {
  char* tempvar = createVar();
  printf("DEFVAR LF@%s\n", tempvar);
  GETTHEM
  printf("EQ LF@%s %s %s\n", tempvar, str1, str2);
  FREETHEM
  TOK_PATH(node) = TOK_PATH(node->left);
  TOK_PATH(node)->value = tempvar;
  TOK_PATH(node)->type = bbool;
  node->isvar = true;
  FREESONS
}
void CONCATSTR(ASSnode_t* node) {
  char* tempvar = createVar();
  printf("DEFVAR LF@%s\n", tempvar);
  GETTHEM
  printf("CONCAT LF@%s %s %s\n", tempvar, str1, str2);
  FREETHEM
  if (node->left->isvar) free(TOK_PATH(node->left)->value);
  TOK_PATH(node) = TOK_PATH(node->left);
  TOK_PATH(node)->value = tempvar;
  node->isvar = true;
}

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
        fprintf(stderr, "SOMETHING WEIRD HAPPENED\n");
        exit(7);
      }
      break;
    case ASSIGN:
      if (node->left->leaf == false) helpsolve(node->left);
      if (node->left->leaf == true) {
        if (TOK_PATH(node->left)->type == ID_variable) {
          ASSIGNVAR(node);
          break;
        }
        if (TOK_PATH(node) == NULL) {
          token_t* dummy = malloc(sizeof(token_t));
          dummy->type = TOK_PATH(node->left)->type;
          TOK_PATH(node) = dummy;
        }

        if (TOK_PATH(node->left)->type == TOK_PATH(node)->type) {
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
        fprintf(stderr, "SOMETHING WEIRD HAPPENED\n");
        exit(7);
      }
      break;
    // case EQ:
    case GT:
    case LT:
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
      printf("EXIT(7)\n");
      printf("LABEL %s\n", label2);
      free(label);
      free(label2);

      if (node->left->leaf == true && node->right->leaf == true) {
        if (TOK_PATH(node->left)->type == TOK_PATH(node->right)->type) {
          if (node->OP == LT) {
            LTCOMP(node);
          } else {
            GTCOMP(node);
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
          if (node->OP == LT) {
            LTCOMP(node);
          } else {
            GTCOMP(node);
          }
          FREETHEM
        }
      } else {
        fprintf(stderr, "SOMETHING WEIRD HAPPENED\n");
        exit(7);
      }
      break;
    case EQ:
      LEAFCHECK
      // dodelat
      if (node->left->leaf == true && node->right->leaf == true) {
        if (TOK_PATH(node->left)->type == TOK_PATH(node->right)->type) {
          EQCOMP(node);
          FREETHEM;
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
          GTCOMP(node);
          FREETHEM
        }
      } else {
        fprintf(stderr, "SOMETHING WEIRD HAPPENED\n");
        exit(7);
      }
      break;
    case CONCAT:
      LEAFCHECK
      if (TOK_PATH(node->left)->type != string ||
          TOK_PATH(node->right) != string) {
        fprintf(stderr, "CONCATENATION OF NON-STRING OPERANDS\n");
        exit(7);
      }
      CONCATSTR(node);
      break;
    case FUNCTIONCALL:
      LEAFCHECK
      break;
    case RYAN_GOSLING:
      LEAFCHECK
      break;
    default:
      fprintf(stderr, "UNEXPECTED OPERAND (%d) HOW DID THAT HAPPEN\n",
              node->OP);
      exit(7);
  }
}

void floatify(token_t* Patrick_Bateman) {
  sprintf(Patrick_Bateman->value, "%a", strtof(Patrick_Bateman->value, NULL));
}

char* CHECKTYPE(ASSnode_t* node) {
  static unsigned int typevarcount;  // LF@tempvar + uint + \0
  char* str = malloc(sizeof(char) * 21);
  if (str == NULL) exit(99);
  sprintf(str, "LF@tempvar%d", typevarcount);
  typevarcount++;
  return str;
}
char* labelgen() {
  static unsigned int labelcount;
  char* str = malloc(sizeof(char) * 16);  // label + uint + \0
  if (str == NULL) exit(99);
  sprintf(str, "label%d", labelcount);
  labelcount++;
  return str;
}

char* checkvar(ASSnode_t* node) {
  char* strptr = malloc(sizeof(char) * 20);  // string@ + uint + \0 idk
  if (strptr == NULL) exit(99);

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

      default:
        fprintf(stderr, "JIHAD JIHAD JIHAD\n");
        exit(17);
    }
  }
}

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
  printf("DEFVAR TF@%%STRING\n");
  printf("READ TF@%%STRING string\n");
  printf("RETURN\n");
}
void readi() {
  printf("LABEL readi\n");
  printf("DEFVAR TF@%%INT\n");
  printf("READ TF@%%INT int\n");
  printf("RETURN\n");
}
void readf() {
  printf("LABEL readf\n");
  printf("DEFVAR TF@%%FLOAT\n");
  printf("READ TF@%%FLOAT float\n");
  printf("RETURN\n");
}
void phpsubstring() {
  printf("LABEL substring");
  printf("\nMOVE LF@\%%STR1 string@a\nDEFVAR TF@lolec");
  printf("\nSTRLEN TF@lolec TF@param1");
  printf("\nDEFVAR TF@zerocheck");
  printf("\nDEFVAR TF@zerocheck2");
  printf("\nDEFVAR TF@ltcheck");
  printf("\nDEFVAR TF@lencheck1");
  printf("\nDEFVAR TF@lencheck2");
  printf("\nDEFVAR TF@i");
  printf("\nDEFVAR TF@L1b");
  printf("\nLT TF@zerocheck TF@param2 int@0");
  printf("\nLT TF@zerocheck2 TF@param3 int@0");
  printf("\nLT TF@ltcheck TF@param3 TF@param2");
  printf("\nLT TF@lencheck1 TF@lolec TF@param2");
  printf("\nGT TF@lencheck2 TF@param3 TF@lolec");
  printf("\nJUMPIFEQ \%%substrfail bool@true TF@zerocheck");
  printf("\nJUMPIFEQ \%%substrfail bool@true TF@zerocheck2");
  printf("\nJUMPIFEQ \%%substrfail bool@true TF@ltcheck");
  printf("\nJUMPIFEQ \%%substrfail bool@true TF@lencheck1");
  printf("\nJUMPIFEQ \%%substrfail bool@true TF@lencheck2");
  printf("\nGETCHAR LF@\%%STR1 TF@param1 TF@param2");
  printf("\nADD TF@param2 TF@param2 int@1");
  printf("\nJUMPIFEQ \%%substrfail TF@param2 TF@param3");
  printf("\nLABEL \%%substrL1");
  printf("\nGETCHAR TF@i TF@param1 TF@param2");
  printf("\nCONCAT LF@\%%STR1 LF@\%%STR1 TF@i");
  printf("\nADD TF@param2 TF@param2 int@1");
  printf("\nJUMPIFNEQ \%%substrL1 TF@param2 TF@param3");
  printf("\nCREATEFRAME");
  printf("\nRETURN");
  printf("\nLABEL \%%substrfail");
  printf("\nMOVE LF@\%%STR1 nil@nil");
  printf("\nCREATEFRAME\nRETURN\n");
}
void print_builtins() {
  printf("JUMP main\n");
  reads();
  readi();
  readf();
  phpsubstring();
  printf("LABEL main\n");
}