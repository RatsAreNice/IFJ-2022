#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Parser.h"

int main(int argc, char const *argv[]) {
  allahprint = fopen("allah.txt", "w");
  fprintf(allahprint, "Parse result: %d", parse());
  fclose(allahprint);
  return 0;
}
