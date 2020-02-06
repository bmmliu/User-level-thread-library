#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>
#include <preempt.h>

int thread3(void* arg)
{
  /*Impletemented an infinite loop in thread so the thread3 will not been printed ant more*/
  preempt_enable();
  for (int i = 1; i < 1000000000; i++) {
    i = i -1;
  }
  uthread_yield();
  preempt_disable();
  printf("thread%d\n", uthread_self());
  return 0;
}

int thread2(void* arg)
{
  preempt_enable();
  uthread_create(thread3, NULL);
  uthread_yield();
  preempt_disable();
  printf("thread%d\n", uthread_self());
  return 0;
}

int thread1(void* arg)
{
  preempt_enable();
  uthread_create(thread2, NULL);
  uthread_yield();
  preempt_disable();
  printf("thread%d\n", uthread_self());
  preempt_enable();
  uthread_yield();
  preempt_disable();
  return 0;
}

int main(void)
{
  preempt_start();
  preempt_enable();
  uthread_join(uthread_create(thread1, NULL), NULL);
  preempt_disable();
  return 0;
}
