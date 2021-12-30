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
  int sret = setslice(getpid(), 14);
  if (sret != 0) {
    printf(1, "XV6_SCHEDULER\t setslice(getpid(), 14) failed\n");
    exit();
  }

  int timeslices[2] = {2, 10};
  int pids[2];

  for (int i = 0; i < 2; ++i) {
    int pid_c = fork2(timeslices[i]);
    if (pid_c < 0) {
      printf(1, "XV6_SCHEDULER\t fork2(%d) failed\n", timeslices[i]);
      exit();
    }

    if (pid_c == 0) {   // child i
      int wc = workload(900000000);   // make sure workload is heavier than parent
      kill(-wc);  // an unelegant way of "using" the workload value to avoid optimized out
      exit();

    } else {  // parent
      pids[i] = pid_c;
    }
  }

  int wp = workload(400063000);
  kill(-wp);  // an unelegant way of "using" the workload value to avoid optimized out

  int waited_pids[2];
  for (int i = 0; i < 2; ++i) {
    waited_pids[i] = wait();
    if (waited_pids[i] < 0) {
      printf(1, "XV6_SCHEDULER\t parent wait failed\n");
      exit();
    }
  }

  if (waited_pids[0] == pids[1] && waited_pids[1] == pids[0]) {
    printf(1, "XV6_SCHEDULER\t SUCCESS\n");
  } else {
    printf(1, "XV6_SCHEDULER\t child 1 (timeslice 10) should finish much"
              " earlier than child 0 (timeslice 2)\n");
  }

  exit();
}
