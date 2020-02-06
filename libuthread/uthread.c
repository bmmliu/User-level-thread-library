#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "context.h"
#include "queue.h"
#include "uthread.h"

/* Private data structure to include all the necessary information */
struct thread_ctr_blk
{
  uthread_t tid;
  uthread_ctx_t context_of_thread;
  void* stack;
  uthread_func_t func_of_thread;
  void* argument;
  int *ret_value;
  struct thread_ctr_blk* parent; //To see if it joins another thread
};

/* Global variable */
struct thread_ctr_blk* executing_thread;
int if_initial = 0;
uthread_t tid_count = 0;
queue_t ready_queue;
queue_t blocked_queue;
queue_t zombie_queue;

/* The initialize function when the first thread is being created*/
int initialize() {
  ready_queue = queue_create();
  blocked_queue = queue_create();
  zombie_queue = queue_create();

  executing_thread = (struct thread_ctr_blk*)malloc(sizeof(struct thread_ctr_blk));
  if (executing_thread == NULL) {
    fprintf(stderr, "Initialization of thread fail\n");
    return -1;
  }

  executing_thread->tid = tid_count;
  tid_count++;
  executing_thread->parent = NULL;
  if_initial = 1;

  return 0;
}

/* To find if a specific thread exist in any queue,
 * to be passed as an argument  */
int tid_exist(void* each_thread, void* TID) {
  struct thread_ctr_blk* newthread = (struct thread_ctr_blk*) each_thread;
  uthread_t* new_tid = (uthread_t*) TID;

  if (newthread->tid == * new_tid){
    return 1;
  } else{
    return 0;
  }
}

/* Implementation of yield */
void uthread_yield(void)
{
  /* Initialize a structure pointer for the pending first thread */
  struct thread_ctr_blk *first_thread_inqueue;
  queue_dequeue(ready_queue, (void**) &first_thread_inqueue);

  /* The thread to be yielded is being copied */
  struct thread_ctr_blk* old_thread = executing_thread;
  /* Throw it at the back of the queue */
  queue_enqueue(ready_queue, (void*)old_thread);
  struct thread_ctr_blk* temp = NULL;
  temp = old_thread;

  /* Now the executing thread is the one that was first on line */
  executing_thread = first_thread_inqueue;
  uthread_ctx_switch(&temp->context_of_thread, &executing_thread->context_of_thread);
}

uthread_t uthread_self(void)
{
  return executing_thread->tid;
}

/* implementation of create thread */
int uthread_create(uthread_func_t func, void *arg)
{
  if(!if_initial) {
    if(initialize() == -1) {
      fprintf(stderr, "Initialization of thread fail\n"); //Error manegement
      return -1;
    }
  }

  /* Creation of the new thread */
  struct thread_ctr_blk *new_thread = (struct thread_ctr_blk*)malloc(sizeof(struct thread_ctr_blk));
  if(new_thread == NULL) {
    fprintf(stderr, "Creation of new thread fail\n");
    return -1;
  }
  /* Set up all the required information */
  new_thread->func_of_thread = func;
  new_thread->argument = arg;
  new_thread->tid = tid_count;
  new_thread->stack = uthread_ctx_alloc_stack();
  new_thread->parent = NULL;

  if(!new_thread->stack) {
    fprintf(stderr, "Creation of stack for new thread fail\n");
    return -1;
  }

  uthread_ctx_init(&new_thread->context_of_thread, (char*)new_thread->stack,
                   new_thread->func_of_thread, new_thread->argument);
  queue_enqueue(ready_queue, (void*)(new_thread));
  tid_count++;

  return new_thread->tid;

}

/* Implementation of exit */
void uthread_exit(int retval)
{
  /* Assign the return value */
  executing_thread->ret_value = &retval;

  /* Make a copy and then put it in zombie */
  struct thread_ctr_blk *old_thread = NULL;
  old_thread = executing_thread;
  queue_enqueue(zombie_queue, (void*)old_thread);

  /* If has parent, has to put parent in ready and unblock it*/
  if(old_thread->parent != NULL) {
    struct thread_ctr_blk *blocked_parent = NULL;
    blocked_parent = old_thread->parent;
    queue_enqueue(ready_queue, (void*)blocked_parent);
    queue_delete(blocked_queue, (void*)blocked_parent);
  }

  /* Now dequeue the first thread in queue and ready to execute */
  struct thread_ctr_blk* new_thread;
  queue_dequeue(ready_queue, (void**)(&new_thread));
  struct thread_ctr_blk* temp = old_thread;
  executing_thread = new_thread;
  uthread_ctx_switch(&temp->context_of_thread, &executing_thread->context_of_thread);
}

/* Implementation of join */
int uthread_join(uthread_t tid, int *retval)
{
  /* Error management */
  if(tid == 0 || tid == executing_thread->tid) {
    fprintf(stderr, "Invalid joined thread\n");
    return -1;
  }

  /* Now try to find where the child is */
  void* if_in_ready = NULL;
  void* if_in_zombie = NULL;
  void* if_in_block = NULL;

  queue_iterate(ready_queue, tid_exist, (void*)(uthread_t*)&tid, (void**)&if_in_ready);
  queue_iterate(zombie_queue, tid_exist, (void*)(uthread_t*)&tid, (void**)&if_in_zombie);
  queue_iterate(blocked_queue, tid_exist, (void*)(uthread_t*)&tid, (void**)&if_in_block);

  if(if_in_zombie == NULL && if_in_ready == NULL && if_in_block == NULL) {
    fprintf(stderr, "Invalid joined thread\n");
    return -1;
  }

  /* Case1: already in zombie */
  if(if_in_zombie != NULL) {
    struct thread_ctr_blk *T2 = (struct thread_ctr_blk *)if_in_zombie;
    if (T2->parent != NULL) {
      fprintf(stderr, "Thread already joined\n");
      return -1;
    }
    T2->ret_value = retval;
    T2->parent = (struct thread_ctr_blk *)executing_thread;
    return 0;
    /* Case2: still active */
  } else if (if_in_ready != NULL){
    struct thread_ctr_blk *T2 = (struct thread_ctr_blk *)if_in_ready;
    if (T2->parent != NULL) {
      fprintf(stderr, "Thread already joined\n");
      return -1;
    }
    T2->ret_value = retval;
    T2->parent = executing_thread;

    /* First, block the running thread */
    struct thread_ctr_blk* old_thread = NULL;
    old_thread = (struct thread_ctr_blk *)executing_thread;
    queue_enqueue(blocked_queue, (void*)old_thread);

    /* Then, execute whatever next in the queue */
    struct thread_ctr_blk* new_thread;
    queue_dequeue(ready_queue, (void**)(&new_thread));
    struct thread_ctr_blk* temp = old_thread;
    executing_thread = new_thread;
    uthread_ctx_switch(&temp->context_of_thread, &executing_thread->context_of_thread);

  } else {
    fprintf(stderr, "Thread want to join already blocked\n");
    return -1;
  }
  return 0;

}
