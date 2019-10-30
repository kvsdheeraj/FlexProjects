#ifndef BINARY_TREE_H
#define BINARY_TREE_H

struct tree_node{
  int value;
  struct tree_node *left_child;
  struct tree_node *right_child;
};

typedef struct tree_node tree_node;

struct tree{
  int size;
  tree_node *root;
};

typedef struct tree tree;

tree * create_tree(void);
bool empty_tree(tree *t);
void insert_tree(tree *t , int value);
void print_tree(tree_node * root);
#endif

