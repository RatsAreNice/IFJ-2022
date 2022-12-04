// Autor : Oliver Nemcek <xnemce08> a Jeffrey Epstein <xdite00>
// parameter funkcie get_token(int skip) udava ci sa preskakuju prazdne znaky.
// ak skip == 0 nepreskakuje sa nic. ak skip == 1, preskakuju sa iba prazdne
// znaky, ak skip != 1 && skip != 0, preskakuju sa prazdne znaky a komentare
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../scanner.h"

int main(int argc, char *argv[]) {
  token_t nigger = get_token(0);

  int ref[118] = {
      prolog1,     prolog2,     ID_variable, assign,      integer,
      semicolon,   function,    ID_function, lbracket,    type,
      ID_variable, rbracket,    colon,       funvoid,     lsetbracket,
      ID_variable, assign,      string,      semicolon,   funif,
      lbracket,    integer,     identity,    integer,     rbracket,
      lsetbracket, ID_variable, assign,      integer,     semicolon,
      ID_function, lbracket,    ID_variable, rbracket,    semicolon,
      rsetbracket, funelse,     lsetbracket, rsetbracket, ID_function,
      lbracket,    ID_variable, rbracket,    semicolon,   ID_variable,
      assign,      integer,     semicolon,   funwhile,    lbracket,
      ID_variable, lte,         integer,     rbracket,    lsetbracket,
      ID_variable, assign,      integer,     semicolon,   ID_variable,
      assign,      ID_variable, plus,        integer,     semicolon,
      ID_function, lbracket,    ID_variable, rbracket,    semicolon,
      ID_function, lbracket,    string,      rbracket,    semicolon,
      ID_variable, assign,      ID_function, lbracket,    rbracket,
      semicolon,   ID_variable, assign,      ID_function, lbracket,
      ID_variable, rbracket,    semicolon,   funif,       lbracket,
      ID_variable, nidentity,   integer,     rbracket,    lsetbracket,
      ID_function, lbracket,    ID_variable, rbracket,    semicolon,
      funreturn,   semicolon,   rsetbracket, funelse,     lsetbracket,
      rsetbracket, rsetbracket, rsetbracket, ID_function, lbracket,
      integer,     rbracket,    semicolon,   ID_function, lbracket,
      ID_variable, rbracket,    semicolon};
  token_t niggarray[118];
  int i = 0;
  do {
    // printf("milujem kokot");
    niggarray[i] = nigger;
    if ((int)niggarray[i].type == ref[i])
      printf("dobre %d \n", i);
    else
      printf("NAPICU %d \n", i);
    i++;
    nigger = get_token(0);
  } while (nigger.type != eof);

  return 0;
}
