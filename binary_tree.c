#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "binary_tree.h"

tree * create_tree(void){
  tree *t = (tree *)malloc(sizeof(tree));
  t->size = 0;
  t->root = NULL;
  return t;
}

bool empty_tree(tree *t){
  return t->size == 0;
}

void insert_tree(tree *t , int value){
  tree_node *tn = (tree_node *)malloc(sizeof(tree_node));
  tn->value = value;
  tn->left_child = tn->right_child = NULL;

  if(empty_tree(t)){
    t->root = tn;
    t->size++;
    return;
  }

  tree_node *root = t->root;
  while(true){
    if(value < root->value){
      if(root->left_child == NULL){
        root->left_child = tn;
        t->size++;
        return;
      }
      else{
        root = root->left_child;
      }
    }
    else if(value > root->value){
      if(root->right_child == NULL){
        root->right_child = tn;
        t->size++;
        return;
      }
      else{
        root = root->right_child;
      }
    }
    else{
      break;
    }
  }
}

void print_tree(tree_node * root){
  if(root == NULL){
    return;
  }
  print_tree(root->left_child);
  printf("%d->",root->value);
  print_tree(root->right_child);
}