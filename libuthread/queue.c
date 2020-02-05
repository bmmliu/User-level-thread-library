#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
/*Underlying data structure to enable the operation to be O(1)*/
struct QueueNode {
  void* value;
  struct QueueNode *next;
};

/*Create a new node when enqueue*/
struct QueueNode *CreateNode(void* new_value) {
  struct QueueNode *newnode = (struct QueueNode *)malloc(sizeof(struct QueueNode));
  newnode->value = new_value;
  newnode->next = NULL;
  return newnode;
}

/*Real data structure to be implemented*/
struct queue {
  struct QueueNode *first;
  struct QueueNode *last;
  int Queuesize;
};

/*Initialization of a queue*/
queue_t queue_create(void) {
  queue_t create_queue = (queue_t)malloc(sizeof(struct queue));
  create_queue->first = NULL;
  create_queue->last = NULL;
  create_queue->Queuesize = 0;
  return create_queue;
}
/*Deallocation of memory*/
int queue_destroy(queue_t queue) {
  if (queue == NULL) {
    return -1;
  } else if (queue->Queuesize == 0) {
    free(queue);
    return 0;
  } else {
    return -1;
  }
}

int queue_enqueue(queue_t queue, void *data) {
  if(queue == NULL || data == NULL) {
    return -1;
  }
  struct QueueNode* new = CreateNode(data);
  if(new == NULL){
    return -1;
  }
  /*If there's currently no node*/
  if(queue->Queuesize == 0) {
    queue->first = new;
    queue->last = new;
    queue->Queuesize++;
    return 0;
  } else {
    queue->last->next = new;
    queue->last = new;
    queue->Queuesize++;
    return 0;
  }
}

int queue_dequeue(queue_t queue, void **data) {
  if(queue == NULL || queue->first == NULL || queue->Queuesize == 0) {
    return -1;
  }
  struct QueueNode* storetemp = queue->first;
  *data = storetemp->value;


  if(queue->Queuesize == 1) {
    queue->first = NULL;
    queue->last = NULL;
  } else {
    queue->first = queue->first->next;
    free(storetemp);
  }

  /*Get rid of the first node*/

  queue->Queuesize--;
  return 0;
}

int queue_delete(queue_t queue, void *data) {
  if(queue == NULL || data == NULL ) {
    return -1;
  }
  struct QueueNode* tocompare = queue->first;
  struct QueueNode* previous = queue->first;
  /*If delete the first node in the queue*/
  if(tocompare->value == data) {
    queue->first = tocompare->next;
    queue->Queuesize--;
    free(previous);
    free(tocompare);
    return 0;
  }
  tocompare = tocompare->next;
  while(tocompare->value != data && tocompare ->next != NULL) {
    tocompare = tocompare->next;
    previous = previous->next;
  }
  /*If can't find the data*/
  if(tocompare->value != data) {
    return -1;
  }
  previous->next = tocompare->next;
  queue->Queuesize--;
  /*Get rid of the node*/
  free(tocompare);
  return 0;
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data) {
  if(queue == NULL || func == NULL) {
    return -1;
  }
  struct QueueNode* ProcessNode = queue->first;
  while(ProcessNode != NULL) {
    if (func(ProcessNode->value, arg)) {
      *data = ProcessNode->value;
    }
    ProcessNode = ProcessNode->next;
  }

  return 0;
}

int queue_length(queue_t queue) {
  if(queue == NULL) {
    return -1;
  }
  return queue->Queuesize;
}