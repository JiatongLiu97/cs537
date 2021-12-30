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


// static struct pstat pstat;

// static int get_schedticks(int pid) {
//   int pret = getpinfo(&pstat);
//   if (pret != 0) {
//     printf(1, "XV6_SCHEDULER\t getpinfo(&pstat) failed\n");
//     exit();
//   }
//   for (int i = 0; i < NPROC; ++i) {
//     if (pstat.inuse[i] == 1 && pstat.pid[i] == pid)
//       return pstat.schedticks[i];
//   }
//   printf(1, "XV6_SCHEDULER\t did not find process %d in the fetched pstat\n", pid);
//   exit();
// }


int
main(int argc, char *argv[])
{
  int mypid = getpid();
  int pid_c0 = -1;

  int sret = setslice(mypid, 2);
  if (sret != 0) {
    printf(1, "XV6_SCHEDULER\t setslice(mypid, 2) failed\n");
    exit();
  }

  for (int i = 0; i < 10; ++i) {
    int pid_c = fork2(i == 0 ? 2 : 4);
    if (pid_c < 0) {
      printf(1, "XV6_SCHEDULER\t fork2(4) failed\n");
      exit();
    }

    if (i == 0)
      pid_c0 = pid_c;

    if (pid_c == 0) {   // child
      if (i == 0)   // one child sleeps for 80
        sleep(80);
      int wc = workload(120000000);   // runs ~55 ticks
      kill(-wc);  // an unelegant way of "using" the workload value to avoid optimized out
      // printf(1, "XV6_SCHEDULER\t child %d: %d\n", i, get_schedticks(getpid()));
      exit();
    }
  }

  int pid_w0 = wait();
  if (pid_w0 != pid_c0) {
    printf(1, "XV6_SCHEDULER\t child 0 should finish before all others"
              " since it should have got 80 compticks\n");
  } else {
    printf(1, "XV6_SCHEDULER\t SUCCESS\n");
  }

  for (int i = 0; i < 9; ++i)
    wait();

  exit();
}
