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


int
main(int argc, char *argv[])
{
  int fret = fork2(7);

  if (fret < 0) {
    printf(1, "XV6_SCHEDULER\t fork2(7) failed\n");

  } else if (fret == 0) {  // child
    int wc = workload(800000000);
    kill(-wc);  // an unelegant way of "using" the workload value to avoid optimized out
    exit();

  } else {  // parent
    int pid_c = fret;
    int timeslice = getslice(pid_c);
    if (timeslice == 7) {
      printf(1, "XV6_SCHEDULER\t SUCCESS\n");
    } else {
      printf(1, "XV6_SCHEDULER\t getslice() on child did not return 7\n");
    }
    wait();
  }
  
  exit();
}
