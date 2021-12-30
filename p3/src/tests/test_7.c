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

static int get_num_switches(int pid) {
  int pret = getpinfo(&pstat);
  if (pret != 0) {
    printf(1, "XV6_SCHEDULER\t getpinfo(&pstat) failed\n");
    exit();
  }
  for (int i = 0; i < NPROC; ++i) {
    if (pstat.inuse[i] == 1 && pstat.pid[i] == pid)
      return pstat.switches[i];
  }
  printf(1, "XV6_SCHEDULER\t did not find process %d in the fetched pstat\n", pid);
  exit();
  return -1;
}


int
main(int argc, char *argv[])
{
  int switches0 = -1, switches1 = -1, switches2 = -1;
  int mypid = getpid();

  switches0 = get_num_switches(mypid);

  // here we should have timeslice == 1
  int w1 = workload(800020009);
  kill(-w1);  // an unelegant way of "using" the workload value to avoid optimized out

  switches1 = get_num_switches(mypid);

  int sret = setslice(mypid, 15);
  if (sret != 0) {
    printf(1, "XV6_SCHEDULER\t setslice(mypid, 15) failed\n");
    exit();
  }

  int w2 = workload(800020010);
  kill(-w2);  // an unelegant way of "using" the workload value to avoid optimized out

  switches2 = get_num_switches(mypid);

  // should have switches2 > switches1 > switches0
  // switches2 - switches1 should be much smaller than switches1 - switches0
  if (!(switches2 > switches1 && switches1 > switches0))
    printf(1, "XV6_SCHEDULER\t should have switches2 > switches1 > switches0\n");
  else if (!((switches1 - switches0) > 5 * (switches2 - switches1)))  // just kindly check for 5 times larger here
    printf(1, "XV6_SCHEDULER\t switches2 - switches1 should be much smaller than switches1 - switches0\n");
  else
    printf(1, "XV6_SCHEDULER\t SUCCESS\n");

  exit();
}
