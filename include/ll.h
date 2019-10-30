#ifndef LL_H
#define LL_H

struct list_node{
  int value;
  struct list_node *next;
  struct list_node *prev;
};

typedef struct list_node list_node;

struct list{
  int size;
  list_node *head;
  list_node *tail;
};

typedef struct list list;

list * create_list(void);
bool empty_list(list *l);
void push_front_list(list *l , int value);
int pop_front_list(list *l);
void push_back_list(list *l, int value);
int pop_back_list(list *l);
#endif