#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "binary_tree.h"

int main(void){

  tree *t = create_tree();
  insert_tree(t,12);
  insert_tree(t,22);
  insert_tree(t,17);
  insert_tree(t,45);
  insert_tree(t,54);
  insert_tree(t,5);

  print_tree(t->root);
  // Print 
}