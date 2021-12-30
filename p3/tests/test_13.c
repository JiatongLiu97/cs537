#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"

#define check(exp, msg) if(exp) {} else {\
  printf(1, "%s:%d check (" #exp ") failed: %s\n", __FILE__, __LINE__, msg);\
  ;}


static int workload(int iters) {
  int i = 0, j = 0;
  while (i < iters) {
    j += i * j + 1;
    i++;
  }
  return j;
}


static struct pstat pstat;
static int compticks = -1;
static int sleepticks = -1;

static void get_comp_info(int pid) {
  int pret = getpinfo(&pstat);
  if (pret != 0) {
    printf(1, "XV6_SCHEDULER\t getpinfo(&pstat) failed\n");
    exit();
  }
  for (int i = 0; i < NPROC; ++i) {
    if (pstat.inuse[i] == 1 && pstat.pid[i] == pid) {
      compticks = pstat.compticks[i];
      sleepticks = pstat.sleepticks[i];
      return;
    }
  }
  printf(1, "XV6_SCHEDULER\t did not find process %d in the fetched pstat\n", pid);
  exit();
}


int
main(int argc, char *argv[])
{
  int mypid = getpid();

  int sret = setslice(mypid, 3);
  if (sret != 0) {
    printf(1, "XV6_SCHEDULER\t setslice(mypid, 3) failed\n");
    exit();
  }

  for (int i = 0; i < 2; ++i) {
    int pid_c = fork2(24);
    if (pid_c < 0) {
      printf(1, "XV6_SCHEDULER\t fork2(24) failed\n");
      exit();
    }

    if (pid_c == 0) {   // child
      int wc = workload(900000000);
      kill(-wc);  // an unelegant way of "using" the workload value to avoid optimized out
      exit();
    }
  }

  get_comp_info(mypid);
  if (compticks != 0 || sleepticks != 0) {
    printf(1, "XV6_SCHEDULER\t sleepticks and compticks should start with 0\n");
    exit();
  }

  // Sleep for several ticks, > child timeslice, < 2 * child timeslice.
  // Depending on how students account for sleepticks, it might be +-1.
  sleep(30);

  get_comp_info(mypid);
  if (sleepticks < 29 || sleepticks > 31) {
    printf(1, "XV6_SCHEDULER\t sleepticks %d does not reflect"
              " sleep(30) - before workload\n", sleepticks);
    exit();
  } else if (compticks != 0) {
    printf(1, "XV6_SCHEDULER\t compticks used %d should be 0 at this time\n", compticks);
    exit();
  }

  // Do a workload, expect all comp ticks to be used.
  int wp = workload(700123450);
  kill(-wp);  // an unelegant way of "using" the workload value to avoid optimized out

  get_comp_info(mypid);
  if (sleepticks < 29 || sleepticks > 31) {
    printf(1, "XV6_SCHEDULER\t sleepticks %d does not reflect"
              " sleep(30) - after workload\n", sleepticks);
    exit();
  } else if (compticks != sleepticks) {
    printf(1, "XV6_SCHEDULER\t compticks used %d does not equal"
              " sleepticks %d\n", compticks, sleepticks);
    exit();
  }
  
  for (int i = 0; i < 2; ++i)
    wait();

  printf(1, "XV6_SCHEDULER\t SUCCESS\n");
  exit();
}
