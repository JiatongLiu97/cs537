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
static int switches[6] = {-1, -2, -3, -4, -5, -6};

static void get_children_pinfo(int pids[], int num) {
  int pret = getpinfo(&pstat);
  if (pret != 0) {
    printf(1, "XV6_SCHEDULER\t getpinfo(&pstat) failed\n");
    exit();
  }
  int found = 0;
  for (int i = 0; i < NPROC; ++i) {
    for (int j = 0; j < num; ++j) {
      if (pstat.inuse[i] == 1 && pstat.pid[i] == pids[j]) {
        switches[j] = pstat.switches[i];
        found++;
        break;
      }
    }
    if (found == num)
      break;
  }
  if (found != num) {
    printf(1, "XV6_SCHEDULER\t did not find all processes in the fetched pstat\n");
    exit();
  }
}


int
main(int argc, char *argv[])
{
  int sret = setslice(getpid(), 11);
  if (sret != 0) {
    printf(1, "XV6_SCHEDULER\t setslice(getpid(), 11) failed\n");
    exit();
  }

  int timeslices[6] = {6, 1, 5, 4, 9, 3};
  int pids[6];

  for (int i = 0; i < 6; ++i) {
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

  int wp = workload(410007002);
  kill(-wp);  // an unelegant way of "using" the workload value to avoid optimized out

  get_children_pinfo(pids, 6);
  int inequal_cnt = 0;
  for (int i = 1; i < 6; ++i) {
    if (switches[i] != switches[0]) {
      printf(1, "XV6_SCHEDULER\t not all processes have same num switches %d != %d\n", switches[i], switches[0]);
      inequal_cnt++;
    }
  }

  if (inequal_cnt == 0)
    printf(1, "XV6_SCHEDULER\t SUCCESS\n");

  for (int i = 0; i < 6; ++i)
    wait();
  exit();
}
