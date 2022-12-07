#include "test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Z test util druheho projektu IAL2. Autor: Michal Koutensky

const char *subtree_prefix = "  |";
const char *space_prefix = "   ";

char *make_prefix(char *prefix, const char *suffix) {
  char *result = (char *)malloc(strlen(prefix) + strlen(suffix) + 1);
  strcpy(result, prefix);
  result = strcat(result, suffix);
  return result;
}

void bst_print_subtree(ASSnode_t *tree, char *prefix, direction_t from) {
  if (tree != NULL) {
    char *current_subtree_prefix = make_prefix(prefix, subtree_prefix);
    char *current_space_prefix = make_prefix(prefix, space_prefix);

    if (from == left) {
      //fprintf(allahprint, "%s\n", current_subtree_prefix);
    }

    bst_print_subtree(
        tree->right,
        from == left ? current_subtree_prefix : current_space_prefix, right);

    fprintf(allahprint, "%s  +-", prefix);
    assprint(tree);
    fprintf(allahprint, "\n");

    bst_print_subtree(
        tree->left,
        from == right ? current_subtree_prefix : current_space_prefix, left);

    if (from == right) {
      fprintf(allahprint, "%s\n", current_subtree_prefix);
    }

    free(current_space_prefix);
    free(current_subtree_prefix);
  }
}

void PRINTTREEFAST(ASSnode_t *tree) {
  fprintf(allahprint, "Binary tree structure:\n");
  fprintf(allahprint, "\n");
  if (tree != NULL) {
    bst_print_subtree(tree, "", none);
  } else {
    fprintf(allahprint, "Tree is empty\n");
  }
  printf("\n");
}