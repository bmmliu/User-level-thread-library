## Project 2 REPORT
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
and set up the context and then enqueue it into the ready queue.
###### **Phase 2.1.2 thread_yield** 
For thread_yield, we first dequeue the ready queue and get the thread to be
executed. Then, we make a copy of the current running thread and enqueue it in
the ready queue. Finally, we make the current running thread to be the thread
that got dequeued in the ready queue and then do the context switch.
###### **Phase 2.1.3 thread_exit** 
For thread_exit, we first assign the return value to the thread. And we create 
a copy of the current running thread and put it into the zombie queue. What is
tricky here is that if it has parent, it has to unblock its parent and put
its parent into ready queue. After taking care of the old running thread, we can 
dequeue the ready state and get the next running thread and do the context switch.
###### **Phase 2.1.4 thread_join** 
There are two cases in this phase. The first case is that when a parent joins 
its child, the child is still active. In this case, we have to block its parent
for a while until its child dies. We determine if it's active by iterating its
PID on the ready queue. In this case, it's in the ready queue and thus active.
Here, we block the current running thread and execute whatver next in the queue.
As soon as the child finishes, the parent will be able to collect its return val.
Another case is that the child is already is zombie state, we can collect the 
return value directly. 

#### **Phase 3.1: preempt library** 
Preempt allows us to use the resources fairly to all threads. In this phase, we 
use two data structure, one of type sigaction to help us deal with the signl,
another of type itimeral to track the time. We first call the sigemptyset to 
clean the signal sent to it. Then we add SIGVTALRM to the sa_mask. Next, we 
use sigprocmask to tell the signal that it can't be blocked since we start the 
preemption. Also, the sa_handler here is basically just yield the function since
that's what we want to do when we receive the signal. For preempt enable and 
disable, we use the same function sigprocmask with different first argument to
achieve the goal.

#### **Phase 3.2: preempt tester**
Based on the given uthread_yield test case, we add the preempt commonds the
code. In the main function, we called the preempt_start() to start the preempt.
And in each time we playing with a thread, we monitor it's running time and 
stop it when it take to much resources. In order to test it that work, we add
an infinite loop in the thread3, so the thread3 will not be completed and the
print of thread3 will no longer appear comparing with the origional uthread_y
ield test.
