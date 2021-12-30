#include "param.h"
struct pstat {
  int inuse[NPROC]; // whether this slot of the process table is in use (1 or 0)
  int pid[NPROC]; // PID of each process
  int timeslice[NPROC]; // number of base ticks this process can run in a timeslice
  int compticks[NPROC]; // number of compensation ticks this process has used
  int schedticks[NPROC];  // total number of timer ticks this process has been scheduled
  int sleepticks[NPROC]; // number of ticks during which this process was blocked
  int switches[NPROC];  // total num times this process has been scheduled
};

