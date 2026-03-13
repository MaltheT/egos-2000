/*
 * (C) 2026, Cornell University
 * All rights reserved.
 *
 * Description: helper functions for process management
 */

#include "process.h"
#include "servers.h"

extern struct process proc_set[MAX_NPROCESS + 1];

static void proc_set_status(int pid, enum proc_status status) {
  for (uint i = 0; i < MAX_NPROCESS; i++)
    if (proc_set[i].pid == pid)
      proc_set[i].status = status;
}

void proc_set_ready(int pid) { proc_set_status(pid, PROC_READY); }
void proc_set_running(int pid) { proc_set_status(pid, PROC_RUNNING); }
void proc_set_runnable(int pid) { proc_set_status(pid, PROC_RUNNABLE); }
void proc_set_pending(int pid) { proc_set_status(pid, PROC_PENDING_SYSCALL); }

void proc_print_stats(int pid) {
#define TICKS_PER_MS 10000 /* QEMU CLINT timer runs at 10 MHz */
  for (u32 i = 0; i < MAX_NPROCESS; i++)
    if (proc_set[i].pid == pid) {
      printf("--- Statistics of PID %d ---\n", pid);
      printf("turnaround_time: %d ms\n",
             (int)(proc_set[i].turnaround_time / TICKS_PER_MS));
      printf("response_time: %d ms\n",
             (int)(proc_set[i].response_time / TICKS_PER_MS));
      printf("CPU_time: %d ms\n", (int)(proc_set[i].CPU_time / TICKS_PER_MS));
      printf("n_timer_interrupts: %d\n", (int)proc_set[i].n_timer_interrupts);
      break;
    }
#undef TICKS_PER_MS
}

int proc_alloc() {
  static uint curr_pid = 0;
  for (uint i = 1; i <= MAX_NPROCESS; i++)
    if (proc_set[i].status == PROC_UNUSED) {
      proc_set[i].pid = ++curr_pid;
      proc_set[i].status = PROC_LOADING;

      /* Student's code goes here (Preemptive Scheduler | System Call). */
      proc_set[i].birth_time = mtime_get();
      proc_set[i].CPU_time = 0;
      proc_set[i].response_time = 0;
      proc_set[i].n_timer_interrupts = 0;

      proc_set[i].wakeup_time = 0;

      if (curr_pid == 1)
        proc_set[i].switch_on_time = mtime_get();

      /* Initialization of lifecycle statistics, MLFQ or process sleep. */
      proc_set[i].mlfq_level = 0;
      proc_set[i].mlfq_runtime = 0;

      /* Student's code ends here. */
      return curr_pid;
    }

  FATAL("proc_alloc: reach the limit of %d processes", MAX_NPROCESS);
}

void proc_free(int pid) {
  /* Student's code goes here (Preemptive Scheduler). */

  /* Print the lifecycle statistics of the terminated process or processes. */
  if (pid != GPID_ALL) {
    for (u32 i = 0; i < MAX_NPROCESS; i++) {
      if (proc_set[i].pid == pid) {
        proc_set[i].turnaround_time = mtime_get() - proc_set[i].birth_time;
        proc_print_stats(pid);
        earth->mmu_free(pid);
        proc_set_status(pid, PROC_UNUSED);
        break;
      }
    }
  } else {
    /* Free all user processes. */
    for (uint i = 0; i < MAX_NPROCESS; i++)
      if (proc_set[i].pid >= GPID_USER_START &&
          proc_set[i].status != PROC_UNUSED) {
        proc_set[i].turnaround_time = mtime_get() - proc_set[i].birth_time;
        proc_print_stats(proc_set[i].pid);
        earth->mmu_free(proc_set[i].pid);
        proc_set[i].status = PROC_UNUSED;
      }
  }
  /* Student's code ends here. */
}

void mlfq_update_level(struct process *p, ulonglong runtime) {
  /* Student's code goes here (Preemptive Scheduler). */
  p->mlfq_runtime += runtime;
  while (p->mlfq_level < MLFQ_NLEVELS - 1 &&
         p->mlfq_runtime >= MLFQ_LEVEL_RUNTIME(p->mlfq_level)) {
    p->mlfq_runtime -= MLFQ_LEVEL_RUNTIME(p->mlfq_level);
    p->mlfq_level++;
  }

  /* Update the MLFQ-related fields in struct process* p after this
   * process has run on the CPU for another runtime microseconds. */

  /* Student's code ends here. */
}

void mlfq_reset_level() {
  /* Student's code goes here (Preemptive Scheduler). */
  /* Reset the level of GPID_SHELL if there is pending keyboard input. */

  if (!earth->tty_input_empty()) {
    for (u32 i = 0; i < MAX_NPROCESS; i++) {
      if (proc_set[i].pid == GPID_SHELL) {
        proc_set[i].mlfq_level = 0;
        proc_set[i].mlfq_runtime = 0;
      }
    }
  }
  static u64 MLFQ_last_reset_time = 0;
  u64 now = mtime_get();
  if (now - MLFQ_last_reset_time >= MLFQ_RESET_PERIOD) {
    MLFQ_last_reset_time = now;
    for (u32 i = 0; i < MAX_NPROCESS; i++) {
      if (proc_set[i].status != PROC_UNUSED) {
        proc_set[i].mlfq_level = 0;
        proc_set[i].mlfq_runtime = 0;
      }
    }
  }
  /* Reset the level of all processes every MLFQ_RESET_PERIOD microseconds. */

  /* Student's code ends here. */
}

void proc_sleep(int pid, uint usec) {
  /* Student's code goes here (System Call & Protection). */

  /* Update the sleep-related fields in the struct process for process pid. */
  for (u32 i = 0; i < MAX_NPROCESS; i++) {
    if (proc_set[i].pid == pid) {
      proc_set[i].wakeup_time = mtime_get() + (u64)usec * 10;
      proc_set[i].status = PROC_SLEEPING;
      break;
    }
  }

  /* Student's code ends here. */
}

void proc_coresinfo() {
  /* Student's code goes here (Multicore & Locks). */

  /* Print out the pid of the process running on each CPU core. */

  /* Student's code ends here. */
}
