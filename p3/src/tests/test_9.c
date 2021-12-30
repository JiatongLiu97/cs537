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
static int schedticks[5] = {-1, -1, -1, -1, -1};
static int switches[5] = {0};

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
        schedticks[j] = pstat.schedticks[i];
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
  int sret = setslice(getpid(), 12);
  if (sret != 0) {
    printf(1, "XV6_SCHEDULER\t setslice(getpid(), 12) failed\n");
    exit();
  }

  int timeslices[5] = {3, 7, 2, 9, 11};
  int pids[5];

  for (int i = 0; i < 5; ++i) {
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

  int wp = workload(420000000);
  kill(-wp);  // an unelegant way of "using" the workload value to avoid optimized out

  get_children_pinfo(pids, 5);
  for (int i = 0; i < 5; ++i) {
    if (schedticks[i] > 0 && schedticks[i] == timeslices[i] * switches[i]) {
      printf(1, "XV6_SCHEDULER\t SUCCESS c%d\n", i);
    } else {
      printf(1, "XV6_SCHEDULER\t child %d has intermediate schedticks %d"
                " != timeslice %d * switches %d\n", i, schedticks[i], timeslices[i], switches[i]);
    }
  }

  for (int i = 0; i < 5; ++i)
    wait();
  exit();
}
