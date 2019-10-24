#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "stack.h"

stack * create_stack(void){
  stack *s = (stack *)malloc(sizeof(stack));
  s->size = 0;
  s->top = NULL;
  return s;
}

bool empty_stack(stack *s){
  return s->size == 0;
}

void push_stack(stack *s , int value){
  stack_node *sn = (stack_node *)malloc(sizeof(stack_node));
  sn->value = value;
  sn->next = s->top;
  s->top = sn;
  s->size++;
}

int pop_stack(stack *s){
  int value = s->top->value;
  stack_node *sn = s->top;
  s->top = s->top->next;
  free(sn);
  s->size--;
  return value;
}