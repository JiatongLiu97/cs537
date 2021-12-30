#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"

#define check(exp, msg) if(exp) {} else {\
  printf(1, "%s:%d check (" #exp ") failed: %s\n", __FILE__, __LINE__, msg);\
  ;}


int
main(int argc, char *argv[])
{
  int pid = getpid();
  check(pid > 0, "getpid() returns non-positive number");

  int sret = setslice(pid, 3);

  if (sret < 0) {
    printf(1, "XV6_SCHEDULER\t setslice(pid, 3) returned -1\n");
  } else {
    struct pstat pstat;
    int pret = getpinfo(&pstat);

    if (pret != 0) {
      printf(1, "XV6_SCHEDULER\t getpinfo(&pstat) returned -1\n");
    } else {
      for (int i = 0; i < NPROC; ++i) {
        if (pstat.inuse[i] == 1 && pstat.pid[i] == pid) {
          if (pstat.timeslice[i] == 3)
            printf(1, "XV6_SCHEDULER\t SUCCESS\n");
          else
            printf(1, "XV6_SCHEDULER\t timeslice of myself in the pstat != 3\n");
          exit();
        }
      }
    }
  }

  printf(1, "XV6_SCHEDULER\t did not find myself in the fetched pstat\n");
  exit();
}
