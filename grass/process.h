#pragma once

#include "egos.h"
#include "syscall.h"
#define MLFQ_NLEVELS 5
#define MLFQ_RESET_PERIOD 10000000             /* 10 seconds */
#define MLFQ_LEVEL_RUNTIME(x) (x + 1) * 100000 /* e.g., 100ms for level 0 */

enum proc_status {
  PROC_UNUSED,
  PROC_LOADING,
  PROC_READY,
  PROC_RUNNING,
  PROC_RUNNABLE,
  PROC_PENDING_SYSCALL,
  PROC_SLEEPING,
};

struct process {
  int pid;
  struct syscall syscall;
  enum proc_status status;
  uint mepc, saved_registers[32];
  /* Student's code goes here (Preemptive Scheduler | System Call). */
  u64 birth_time;
  u64 turnaround_time;
  u64 response_time;
  u64 switch_on_time;
  u64 CPU_time;
  u8 n_timer_interrupts;

  u64 wakeup_time;

  /* Add new fields for lifecycle statistics, MLFQ or process sleep. */
  u8 mlfq_level;
  u64 mlfq_runtime;

  /* Student's code ends here. */
};
#define MAX_NPROCESS 16

ulonglong mtime_get();

int proc_alloc();
void proc_free(int);
void proc_set_ready(int);
void proc_set_running(int);
void proc_set_runnable(int);
void proc_set_pending(int);
void proc_print_stats(int);

void mlfq_reset_level();
void mlfq_update_level(struct process *p, ulonglong runtime);
void proc_sleep(int pid, uint usec);
void proc_coresinfo();

extern uint core_to_proc_idx[NCORES];
