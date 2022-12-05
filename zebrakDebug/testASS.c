#include "../program/ASS.h"

int main() {
  // generate code for 5+7-3-4+2

  ASSnode_t** root;

  token_t five;
  five.type = integer;
  five.value = "5";
  token_t seven;
  seven.type = integer;
  seven.value = "7";
  token_t three;
  three.type = integer;
  three.value = "3";
  token_t four;
  four.type = integer;
  four.value = "4";
  token_t two;
  two.type = integer;
  two.value = "2";
  token_t batukam;
  batukam.type = ffloat;
  batukam.value = "kredenc";

  ASSnode_t* fiveleaf = makeLeaf(&five);
  ASSnode_t* sevenleaf = makeLeaf(&seven);
  ASSnode_t* threeleaf = makeLeaf(&three);
  ASSnode_t* fourleaf = makeLeaf(&four);
  ASSnode_t* twoleaf = makeLeaf(&two);

  ASSnode_t* trollec = makeTree(ADD, fiveleaf, sevenleaf);
  ASSnode_t* lolec = makeTree(SUB, trollec, threeleaf);

  // SUBTRACTION EXAMPLE
  ASSnode_t* holec = makeTree(SUB, lolec, fourleaf);

  // ADDITION EXAMPLE
  ASSnode_t* molec = makeTree(ADD, holec, twoleaf);

  // ASSIGN EXAMPLE
  ASSnode_t* golec = makeTree(ASSIGN, molec, molec);
  golec->Patrick_Bateman = &batukam;

  root = &golec;
  print_code(root);

  return 0;
}