#ifndef STACK_H
#define STACK_H

struct stack_node {
  int value;
  struct stack_node *next;
};

typedef struct stack_node stack_node;


struct stack{
    int size;
    stack_node *top;
};

typedef struct stack stack;

stack * create_stack(void);
bool empty_stack(stack *s);
void push_stack(stack *s , int value);
int pop_stack(stack *s);

#endif