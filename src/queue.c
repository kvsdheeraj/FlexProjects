#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "queue.h"

queue * create_queue(void){
  queue *q = (queue *)malloc(sizeof(queue));
  q->size = 0;
  q->front = NULL;
  q->back = NULL;
  return q;
}

bool empty_queue(queue *q){
  return q->size == 0;
}

void push_queue(queue *q, int value){

    queue_node *qn = (queue_node *)malloc(sizeof(queue_node));
    qn->value = value;
    qn->next = NULL;

    if(q->size == 0){
      q->front = q->back = qn;
    }
    else{
      q->back->next = qn;
      q->back = qn;
    }
    q->size++;
}

int pop_queue(queue *q){
  if(empty_queue(q)){
    printf("\nERR: List is empty");
    return 0;
  }
  int value = q->front->value;
  queue_node *qn = q->front;
  q->front = q->front->next;
  free(qn);
  q->size--;
  return value;
}