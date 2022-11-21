#ifndef SYMTABLE_H
#define SYMTABLE_H
typedef struct funData funData_t;
typedef struct bst_node bst_node_t;

void bst_init(bst_node_t **tree);

bst_node_t* bst_search(bst_node_t *tree, char* key);

int bst_insert(bst_node_t **tree, char* key, int type, funData_t* funData);

void bst_dispose(bst_node_t **tree);
void bst_delete(bst_node_t **tree, char* key);
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree);
#endif