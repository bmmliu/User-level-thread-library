#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue.h>

#define TEST_ASSERT(assert)   \
do {      \
 printf("ASSERT: " #assert " ... "); \
 if (assert) {    \
  printf("PASS\n");  \
 } else {    \
  printf("FAIL\n");  \
  exit(1);   \
 }     \
} while(0)

/* 1.Create */
void test_create(void)
{
  fprintf(stderr, "*** TEST create ***\n");

  TEST_ASSERT(queue_create() != NULL);
}

/* 2.Enqueue/Dequeue simple */
void test_queue_simple(void)
{
  int data = 3, *ptr;
  queue_t q;

  fprintf(stderr, "*** TEST queue_simple ***\n");

  q = queue_create();
  queue_enqueue(q, &data);
  queue_dequeue(q, (void**)&ptr);
  TEST_ASSERT(ptr == &data);
  queue_destroy(q);
}

/* 3.Destroy the queue if queue is null*/
void test_queue_destroy_if_null(void)
{
  fprintf(stderr, "*** TEST queue destroy if null ***\n");

  queue_t q = NULL;
  int retval = 0;
  /* test if queue is null */
  retval = queue_destroy(q);
  TEST_ASSERT(retval == -1);

}

/* 4. Destroy the queue if queue is not empty */
void test_queue_destroy_if_not_empty(void)
{
  fprintf(stderr, "*** TEST queue destroy if not empty ***\n");

  queue_t q = NULL;
  int retval;
  q = queue_create();
  int data = 5;
  queue_enqueue(q, &data);
  retval = queue_destroy(q);
  TEST_ASSERT(retval == -1);
}

/* 5. Now test if queue is being destroyed successfully */
void test_queue_destroy_OK(void)
{
  fprintf(stderr, "*** TEST queue destroy if success ***\n");

  queue_t q = NULL;
  int retval = 0;
  int sizeofqueue = 0;
  q = queue_create();
  int data = 5, *ptr;

  queue_enqueue(q, &data);
  queue_dequeue(q, (void**)&ptr);
  sizeofqueue = queue_length(q);
  TEST_ASSERT(sizeofqueue == 0);
  retval = queue_destroy(q);
  TEST_ASSERT(retval == 0);

}

/* 6.Test if enqueue an empty queue */
void test_enqueue_empty(void)
{
  fprintf(stderr, "*** TEST enqueue if queue is empty ***\n");

  queue_t q = NULL;
  int retval = 0;
  int sizeofqueue = 0;
  q = queue_create();
  int data = 5, *ptr;

  retval = queue_enqueue(q, &data);
  sizeofqueue = queue_length(q);
  TEST_ASSERT(sizeofqueue == 1);
  TEST_ASSERT(retval == 0);
  retval = queue_dequeue(q, (void**)&ptr);
  TEST_ASSERT(retval == 0);
  TEST_ASSERT(ptr == &data);
  queue_destroy(q);

}

/* 7. Test if enqueue an non-empty queue */
void test_enqueue_nonempty(void)
{
  fprintf(stderr, "*** TEST enqueue if queue is non-empty ***\n");

  queue_t q = NULL;
  int retval = 0;
  int sizeofqueue = 0;
  q = queue_create();
  int data0 = 5, *ptr0;
  int data1 = 4, *ptr1;
  int data2 = 3, *ptr2;

  queue_enqueue(q, &data0);
  queue_enqueue(q, &data1);
  retval = queue_enqueue(q, &data2);
  sizeofqueue = queue_length(q);
  TEST_ASSERT(retval == 0);
  TEST_ASSERT(sizeofqueue == 3);
  queue_dequeue(q, (void**)&ptr0);
  queue_dequeue(q, (void**)&ptr1);
  queue_dequeue(q, (void**)&ptr2);
  TEST_ASSERT(ptr0 == &data0);
  TEST_ASSERT(ptr1 == &data1);
  TEST_ASSERT(ptr2 == &data2);
  sizeofqueue = queue_length(q);
  TEST_ASSERT(sizeofqueue == 0);
  queue_destroy(q);
}

/* 8. Test if enqueue a null queue */
void test_enqueue_null_queue(void)
{
  fprintf(stderr, "*** TEST enqueue if queue is null ***\n");

  queue_t q = NULL;
  int retval = 0;
  int data = 5;

  retval = queue_enqueue(q, &data);
  TEST_ASSERT(retval == -1);
}

/* 9. Test if enqueue a null data */
void test_enqueue_null_data(void)
{
  fprintf(stderr, "*** TEST enqueue if data is null ***\n");

  queue_t q = NULL;
  q = queue_create();
  void *data = NULL;
  int retval = 0;

  retval = queue_enqueue(q, data);
  TEST_ASSERT(retval == -1);
  queue_destroy(q);
}

/* 10. Test if dequeue a null queue */
void test_dequeue_null_queue(void)
{
  fprintf(stderr, "*** TEST dequeue if queue is null ***\n");

  queue_t q = NULL;
  void *ptr = NULL;
  int retval = 0;

  retval = queue_dequeue(q, (void**) &ptr);
  TEST_ASSERT(retval == -1);
}

/* 11. Test if dequeue an empty queue */
void test_dequeue_empty_queue(void)
{
  fprintf(stderr, "*** TEST dequeue an empty queue ***\n");

  queue_t q = NULL;

  int *ptr1;
  int retval = 0;
  int sizeofqueue = 0;

  q = queue_create();

  retval = queue_dequeue(q, (void**)&ptr1);
  sizeofqueue = queue_length(q);
  TEST_ASSERT(sizeofqueue == 0);
  TEST_ASSERT(retval == -1);
  queue_destroy(q);
}

/* 12. Test if dequeue a null data */
void test_dequeue_null_data(void)
{
  fprintf(stderr, "*** TEST dequeue a null data ***\n");

  queue_t q = NULL;
  q = queue_create();
  void *data = NULL;
  int retval = 0;

  retval = queue_dequeue(q, &data);
  TEST_ASSERT(retval == -1);
  queue_destroy(q);

}

/* 13. Test if delete in a null queue */
void test_delete_null_queue(void)
{
  fprintf(stderr, "*** TEST delete in a null queue ***\n");

  queue_t q = NULL;
  int data = 5;
  int retval = 0;

  retval = queue_delete(q, &data);
  TEST_ASSERT(retval == -1);
}

/* 14. Test if delete in a null data */
void test_delete_null_data(void)
{
  fprintf(stderr, "*** TEST delete in a null data ***\n");

  queue_t q = NULL;
  int *data = NULL;
  int retval = 0;

  q = queue_create();
  retval = queue_delete(q, data);
  TEST_ASSERT(retval == -1);
  queue_destroy(q);
}

/* 15. Test delete if data was not found */
void test_delete_nonexist_data(void)
{
  fprintf(stderr, "*** TEST delete if data does not exist ***\n");

  queue_t q = NULL;
  int retval = 0;
  int sizeofqueue = 0;
  q = queue_create();
  int data0 = 5, ptr0;
  int data1 = 4, ptr1;
  int data2 = 3, ptr2;
  int datanoexist = 2;

  queue_enqueue(q, &data0);
  queue_enqueue(q, &data1);
  queue_enqueue(q, &data2);
  sizeofqueue = queue_length(q);
  TEST_ASSERT(sizeofqueue == 3);
  retval = queue_delete(q, &datanoexist);
  TEST_ASSERT(retval == -1);
  queue_dequeue(q, (void**) &ptr0);
  queue_dequeue(q, (void**) &ptr1);
  queue_dequeue(q, (void**) &ptr2);
  queue_destroy(q);
}


/* 16. Test if delte working */
void test_delete(void)
{
  fprintf(stderr, "*** TEST delete middle ***\n");

  queue_t q = NULL;
  int retval = 0;
  int sizeofqueue = 0;
  q = queue_create();
  int data0 = 5, *ptr0;
  int data1 = 4, *ptr1;
  int data2 = 3;
  int data3 = 2, *ptr3;
  queue_enqueue(q, &data0);
  queue_enqueue(q, &data1);
  queue_enqueue(q, &data2);
  queue_enqueue(q, &data3);
  sizeofqueue = queue_length(q);
  TEST_ASSERT(sizeofqueue == 4);
  retval = queue_delete(q, &data2);
  TEST_ASSERT(retval == 0);
  sizeofqueue = queue_length(q);
  TEST_ASSERT(sizeofqueue == 3);
  queue_dequeue(q, (void**)&ptr0);
  TEST_ASSERT(ptr0 == &data0);
  queue_dequeue(q, (void**)&ptr1);
  TEST_ASSERT(ptr1 == &data1);
  queue_dequeue(q, (void**)&ptr3);
  TEST_ASSERT(ptr3 == &data3);
  queue_destroy(q);


}

void final_test(void) {
  fprintf(stderr, "*** final test ***\n");
  queue_t q = NULL;

  q = queue_create();
  int data0 = 5, *ptr0;
  int data1 = 4, *ptr1;
  int data2 = 3, *ptr2;
  int data3 = 2, *ptr3;
  int data4 = 1, *ptr4;
  queue_enqueue(q, &data0);
  queue_enqueue(q, &data1);
  queue_enqueue(q, &data2);
  queue_enqueue(q, &data3);
  queue_enqueue(q, &data4);
  queue_dequeue(q, (void**) &ptr0);
  queue_dequeue(q, (void**) &ptr1);
  queue_dequeue(q, (void**) &ptr2);
  queue_dequeue(q, (void**) &ptr3);
  queue_dequeue(q, (void**) &ptr4);
  TEST_ASSERT(ptr0 == &data0);
  TEST_ASSERT(ptr1 == &data1);
  TEST_ASSERT(ptr2 == &data2);
  TEST_ASSERT(ptr3 == &data3);
  TEST_ASSERT(ptr4 == &data4);
  queue_destroy(q);

}
static int inc_item(void *data, void *arg)
{
  int *a = (int*)data;
  int inc = (int)(long)arg;

  *a += inc;

  return 0;
}

/* Callback function that finds a certain item according to its value */
static int find_item(void *data, void *arg)
{
  int *a = (int*)data;
  int match = (int)(long)arg;

  if (*a == match)
    return 1;

  return 0;
}

void test_iterator(void)
{
  fprintf(stderr, "*** iteration test ***\n");
  queue_t q;
  int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int i;
  int *ptr;

  /* Initialize the queue and enqueue items */
  q = queue_create();
  for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
    queue_enqueue(q, &data[i]);

  /* Add value '1' to every item of the queue */
  queue_iterate(q, inc_item, (void*)1, NULL);
  assert(data[0] == 2);

  /* Find and get the item which is equal to value '5' */
  ptr = NULL;
  queue_iterate(q, find_item, (void*)5, (void**)&ptr);
  TEST_ASSERT(ptr != NULL);
  TEST_ASSERT(*ptr == 5);
  TEST_ASSERT(ptr == &data[3]);
}

int main(void)
{
  test_create();
  test_queue_simple();
  test_queue_destroy_if_null();
  test_queue_destroy_if_not_empty();
  test_queue_destroy_OK();
  test_enqueue_empty();
  test_enqueue_nonempty();
  test_enqueue_null_queue();
  test_enqueue_null_data();
  test_dequeue_null_queue();
  test_dequeue_empty_queue();
  test_dequeue_null_data();
  test_delete_null_queue();
  test_delete_null_data();
  test_delete_nonexist_data();
  test_delete();
  final_test();
  test_iterator();

  return 0;
}