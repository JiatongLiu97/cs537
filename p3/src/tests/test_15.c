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
static int compticks = -1;
static int sleepticks = -1;

static void get_comp_info(int pid) {
  int pret = getpinfo(&pstat);
  if (pret != 0) {
    printf(1, "XV6_SCHEDULER\t getpinfo(&pstat) failed\n");
    exit();
  }
  for (int i = 0; i < NPROC; ++i) {
    if (pstat.inuse[i] == 1 && pstat.pid[i] == pid) {
      compticks = pstat.compticks[i];
      sleepticks = pstat.sleepticks[i];
      return;
    }
  }
  printf(1, "XV6_SCHEDULER\t did not find process %d in the fetched pstat\n", pid);
  exit();
}


int
main(int argc, char *argv[])
{
  int sret = setslice(getpid(), 3);
  if (sret != 0) {
    printf(1, "XV6_SCHEDULER\t setslice(getpid(), 3) failed\n");
    exit();
  }

  int pid_c0 = fork2(3);
  if (pid_c0 < 0) {
    printf(1, "XV6_SCHEDULER\t fork2(3) failed\n");
    exit();
  }

  if (pid_c0 == 0) {  // child 0
    int mypid = getpid();

    int sret = setslice(mypid, 3);
    if (sret != 0) {
      printf(1, "XV6_SCHEDULER\t setslice(mypid, 3) failed\n");
      exit();
    }

    get_comp_info(mypid);
    if (compticks != 0 || sleepticks != 0) {
      printf(1, "XV6_SCHEDULER\t sleepticks and compticks should start with 0\n");
      exit();
    }

    // Sleep for enough number of ticks.
    // Depending on how students account for sleepticks, it might be +-1.
    sleep(64);

    get_comp_info(mypid);
    if (sleepticks < 63 || sleepticks > 65) {
      printf(1, "XV6_SCHEDULER\t sleepticks %d does not reflect"
                " sleep(64) - before workload\n", sleepticks);
      exit();
    } else if (compticks != 0) {
      printf(1, "XV6_SCHEDULER\t compticks used %d should be 0 at this time\n", compticks);
      exit();
    }

    // Repeatedly doing very small workload, expect partial comp ticks used.
    int compticks_used = 0;
    while (compticks_used < 10) {   // stop early just at 10
      int w0 = workload(100000);
      kill(-w0);  // an unelegant way of "using" the workload value to avoid optimized out

      get_comp_info(mypid);
      if (compticks > compticks_used)
        compticks_used = compticks;
    }

    if (compticks_used >= 64) {
      printf(1, "XV6_SCHEDULER\t used up all compensation ticks, should not happen\n");
      exit();
    }

    // Go to sleep again, should drop all unused comp ticks.
    sleep(4);

    // Then, child 0 forks child 1...
    int pid_c1 = fork2(3);
    if (pid_c1 < 0) {
      printf(1, "XV6_SCHEDULER\t fork2(3) failed\n");
      exit();
    }

    if (pid_c1 == 0) {   // child 1
      int w1 = workload(123456);
      kill(-w1);   // an unelegant way of "using" the workload value to avoid optimized out
      printf(1, "XV6_SCHEDULER\t inner child 1 workload DONE\n");
      exit();

    } else {  // child 0
      // Do a heavy workload. Should not have accumulated comp ticks.
      int w0 = workload(560080900);
      kill(-w0);  // an unelegant way of "using" the workload value to avoid optimized out

      printf(1, "XV6_SCHEDULER\t outer child 0 workload DONE\n");

      get_comp_info(mypid);
      if (compticks <= compticks_used) {
        printf(1, "XV6_SCHEDULER\t the second sleep(4) did not grant any compensation ticks\n");
        exit();
      } else if (compticks > compticks_used + 5) {
        printf(1, "XV6_SCHEDULER\t at least %d compensation ticks accumulated on the next cycle\n",
               compticks - compticks_used - 5);
        exit();
      }

      wait();
      exit();
    }

  } else {  // parent
    wait();
  }

  // Expects inner child 1 DONE first, then outer child 0 DONE.
  exit();
}
