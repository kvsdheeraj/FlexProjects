#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "ll.h"

list * create_list(void){
  list * l = (list *)malloc(sizeof(list));
  l->head = NULL;
  l->tail = NULL;
  l->size = 0;
  return l;
}

bool empty_list(list *l){
  return l->size == 0;
}

void push_front_list(list *l , int value){
  list_node *ln = (list_node *)malloc(sizeof(list_node));
  ln->value = value;
  ln->next = ln->prev = NULL;

  if(empty_list(l)){
    l->head = ln;
    l->size++;
    return;
  }

  ln->next = l->head;
  ln->prev = NULL;
  l->head = ln;
  l->size++;
}

int pop_front_list(list *l){
  if(empty_list(l)){
    printf("ERR: List is empty\n");
    return 0;
  }

  int value = l->head->value;
  list_node *ln = l->head;

  if(l->size == 1){
    l->head = l->tail = NULL;
    free(l->head);
    l->size--;
    return value;
  }

  l->head = l->head->next;
  l->head->prev = NULL;
  free(ln);
  l->size--;
  return value;
}

void push_back_list(list *l, int value){
  list_node *ln = (list_node *)malloc(sizeof(list_node));
  ln->value = value;
  ln->next = ln->prev = NULL;
  if(empty_list(l)){
    l->head = l->tail = ln;
    l->size++;
    return;
  }

  ln->prev = l->tail;
  l->tail->next = ln;
  l->tail = ln;
  l->size++;
}

int pop_back_list(list *l){
  if(empty_list(l)){
    printf("ERR: List is empty\n");
    return 0;
  }

  int value = l->tail->value;
  list_node *ln = l->tail;
  if(l->size == 1){
    free(l->tail);
    l->head = NULL;
    l->tail = NULL;
    l->size--;
    return value;
  }

  l->tail = l->tail->prev;
  l->tail->next = NULL;
  free(ln);
  l->size--;
  return value;
}



















































































