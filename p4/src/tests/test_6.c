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
  // struct pstat pstat;
  int pret = getpinfo((struct pstat *)0x6fffffff);

  if (pret == -1) {
    printf(1, "XV6_SCHEDULER\t SUCCESS\n");
  } else {
    printf(1, "XV6_SCHEDULER\t getpinfo(0x6fffffff) should fail and return -1\n");
  }
  
  exit();
}
