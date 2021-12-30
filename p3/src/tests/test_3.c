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
  int fret = fork2(-5);
  int gret = getslice(-2);

  if (fret == -1 && gret == -1) {
    printf(1, "XV6_SCHEDULER\t SUCCESS\n");
  } else {
    if (fret != -1)
      printf(1, "XV6_SCHEDULER\t fork2(-5) should fail and return -1\n");
    if (gret != -1)
      printf(1, "XV6_SCHEDULER\t getslice(-2) should fail and return -1\n");
  }
  
  exit();
}
