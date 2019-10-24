#ifndef QUEUE_H
#define QUEUE_H

struct queue_node{
  int value;
  struct queue_node *next;
};

typedef struct queue_node queue_node;

struct queue{
  int size;
  queue_node *front;
  queue_node *back;
};

typedef struct queue queue;

queue * create_queue(void);
bool empty_queue(queue *q);
void push_queue(queue *q, int value);
int pop_queue(queue *q);
#endif