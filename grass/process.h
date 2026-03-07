#pragma once

#include "egos.h"
#include "syscall.h"
#include "utils/types.h"

enum proc_status {
  PROC_UNUSED,
  PROC_LOADING,
  PROC_READY,
  PROC_RUNNING,
  PROC_RUNNABLE,
  PROC_PENDING_SYSCALL
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

  /* Add new fields for lifecycle statistics, MLFQ or process sleep. */

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
