#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>

#include "preempt.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

struct sigaction signal_action;
struct itimerval time_intrval;

void signal_handler() {
  uthread_yield();
}
void preempt_disable(void) {
  sigprocmask(SIG_BLOCK, &signal_action.sa_mask, 0);
}

void preempt_enable(void) {
  sigprocmask(SIG_UNBLOCK, &signal_action.sa_mask, 0);
}

void preempt_start(void) {
  time_intrval.it_interval.tv_usec = 100 * HZ;
  time_intrval.it_interval.tv_sec = 0;
  time_intrval.it_value.tv_usec = 100 * HZ;
  time_intrval.it_interval.tv_sec = 0;
  sigemptyset(&signal_action.sa_mask);
  sigaddset(&signal_action.sa_mask, SIGVTALRM);
  sigprocmask(SIG_UNBLOCK, &signal_action.sa_mask, 0);
  signal_action.sa_handler = signal_handler;
  if (sigaction(SIGVTALRM, &signal_action, 0)) {
    fprintf(stderr, "*** sigaction fault ***\n");
    exit(1);
  }
  if (setitimer(ITIMER_VIRTUAL, &time_intrval, 0)) {
    fprintf(stderr, "*** setitimer fault ***\n");
    exit(1);
  }

}
