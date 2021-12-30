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
  int sret1 = setslice(-4, 8);
  int sret2 = setslice(getpid(), -9);

  if (sret1 == -1 && sret2 == -1) {
    printf(1, "XV6_SCHEDULER\t SUCCESS\n");
  } else {
    if (sret1 != -1)
      printf(1, "XV6_SCHEDULER\t setslice(-4, 8) should fail and return -1\n");
    if (sret2 != -1)
      printf(1, "XV6_SCHEDULER\t setslice(getpid(), -9) should fail and return -1\n");
  }
  
  exit();
}
