## Project 1 REPORT
In this program, we are asked to implement a basic user-level thread library
with scheduling the thread by FIFO strategy. Since we implemented the program 
based on the suggested phase given on the promopt, we would illustrate them 
orderly according to the phase number.

#### **Phase 1.1: Queue API** 
For this phase, we are implementing a queue that is expected to do the most 
operation in O(1). Thus, we decide to use singly linked list to help implement
it. We create a data structure called queue, which has the pointer to the first
ndoe and pointer to the last node. In the meantime, we create a data structure
for each node in the queue, containing its value and pointer to the next node.
By doing this, we acheiving the goal of implementing queue_create, queue_destroy
,queue_enqueue, queue_dequeue by playing around with the pointer and it's only
O(1). For queue_delete, we need do some iterations to find the node to delete if
existed. As for iteration, we have to iterate through the whole queue.

#### **Phase 1.2: Queue Tester** 
We made 18 cases in total to cover all potential queue operations to make sure
our queue is working as intended. Here, we go through the queue.h again to find 
any possibiltiy. Take queue_delete as an example, we have four test cases
for it.TEST_DELETE_NULL_DATA,TEST_DELETE_NULL_QUEUE,TEST_DELETE_NONEXIST and 
TEST_DELETE_WORK to test if all the necessary error management are being cared.
We make sure we pass all the test cases we created before heading to next phase.

#### **Phase 2.1: uthread library** 
For this library, we create a data structure called thread_ctr_blk to hold 
all the required information realted to a thread. Also, we have several
global variables like executing_thread, three queues each individually represent
a state. Other than these, we have two helper functions, one to initialize the
first thread and all the queues, another one to help us find if the tid exist
in a queue.
###### **Phase 2.1.1 thread_create** 
For thread_create, we simply make a new thread and initialize all its information
and then enqueue it into the ready queue.
###### **Phase 2.1.2 thread_yield** 
For thread_yield, we first dequeue the ready queue and get the thread to be
executed. Then, we make a copy of the current running thread and enqueue it in
the ready queue. Finally, we make the current running thread to be the thread
that got dequeued in the ready queue and then do the context switch.
###### **Phase 2.1.3 thread_exit** 
For thread_exit, we first assign the return value to the thread. And we create 
a copy of the current running thread and put it into the zombie queue. What is
tricky in this part is that if it has child, it has to unblock its child and put
its child into ready queue. After taking care of the old running thread, we can 
dequeue the ready state and get the next running thread and do the context switch.


