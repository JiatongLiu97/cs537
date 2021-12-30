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

static int get_compticks(int pid) {
  int pret = getpinfo(&pstat);
  if (pret != 0) {
    printf(1, "XV6_SCHEDULER\t getpinfo(&pstat) failed\n");
    exit();
  }
  for (int i = 0; i < NPROC; ++i) {
    if (pstat.inuse[i] == 1 && pstat.pid[i] == pid)
      return pstat.compticks[i];
  }
  printf(1, "XV6_SCHEDULER\t did not find process %d in the fetched pstat\n", pid);
  exit();
}


int
main(int argc, char *argv[])
{
  int compticks0 = -1, compticks1 = -2;
  int mypid = getpid();

  compticks0 = get_compticks(mypid);
  if (compticks0 < 0) {
    printf(1, "XV6_SCHEDULER\t fetched invalid compticks %d from pstat\n", compticks0);
    exit();
  }

  for (int iter = 0; iter < 4; ++iter) {
    // Sleep for 5 ticks.
    sleep(5);

    // Do workload at wakeup.
    int wp = workload(520000000);
    kill(-wp);  // an unelegant way of "using" the workload value to avoid optimized out

    compticks1 = get_compticks(mypid);
    if (compticks0 < 0) {
      printf(1, "XV6_SCHEDULER\t fetched invalid compticks %d from pstat\n", compticks1);
      exit();
    }

    if (compticks1 <= compticks0) {
      printf(1, "XV6_SCHEDULER\t iter %d: compticks %d -> %d did not progress\n",
                iter, compticks0, compticks1);
    } else {
      printf(1, "XV6_SCHEDULER\t SUCCESS iter %d\n", iter);
    }

    compticks0 = compticks1;
  }

  exit();
}
