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
static int schedticks_c = -1;
static int switches_c = 0;

static void get_child_pinfo(int pid) {
  int pret = getpinfo(&pstat);
  if (pret != 0) {
    printf(1, "XV6_SCHEDULER\t getpinfo(&pstat) failed\n");
    exit();
  }
  for (int i = 0; i < NPROC; ++i) {
    if (pstat.inuse[i] == 1 && pstat.pid[i] == pid) {
      schedticks_c = pstat.schedticks[i];
      switches_c = pstat.switches[i];
      return;
    }
  }
  printf(1, "XV6_SCHEDULER\t did not find process %d in the fetched pstat\n", pid);
  exit();
}


int
main(int argc, char *argv[])
{
  int sret = setslice(getpid(), 8);
  if (sret != 0) {
    printf(1, "XV6_SCHEDULER\t setslice(getpid(), 8) failed\n");
    exit();
  }

  int pid_c = fork2(7);
  if (pid_c < 0) {
    printf(1, "XV6_SCHEDULER\t fork2(7) failed\n");
    exit();
  }

  if (pid_c == 0) {   // child
    int w1 = workload(900000000);   // make sure child executes a workload heavier than parent
    kill(-w1);  // an unelegant way of "using" the workload value to avoid optimized out
    exit();

  } else {  // parent
    int w2 = workload(420000000);
    kill(-w2);  // an unelegant way of "using" the workload value to avoid optimized out

    get_child_pinfo(pid_c);
    if (schedticks_c > 0 && schedticks_c == 7 * switches_c)
      printf(1, "XV6_SCHEDULER\t SUCCESS\n");
    else
      printf(1, "XV6_SCHEDULER\t child has intermediate schedticks %d"
                " != timeslice 7 * switches %d\n", schedticks_c, switches_c);

    wait();
  }

  exit();
}
