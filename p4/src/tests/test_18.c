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
}


int
main(int argc, char *argv[])
{
  int switches0 = -2, switches1 = -1;
  int mypid = getpid();
  
  switches0 = get_num_switches(mypid);

  int sret = setslice(mypid, 100);
  if (sret != 0) {
    printf(1, "XV6_SCHEDULER\t setslice(mypid, 100) failed\n");
    exit();
  }

  int w0 = workload(20000000);  // a small one
  kill(-w0);  // an unelegant way of "using" the workload value to avoid optimized out

  switches1 = get_num_switches(mypid);

  if (switches0 < 0 || switches1 < 0) {
    printf(1, "XV6_SCHEDULER\t invalid num switches value fetched from pstat\n");
    exit();
  }
  
  if (switches1 != switches0) {
    printf(1, "XV6_SCHEDULER\t scheduler cycle switch happened after setslice(mypid, 100)\n");
  } else {
    printf(1, "XV6_SCHEDULER\t SUCCESS\n");
  }

  exit();
}
