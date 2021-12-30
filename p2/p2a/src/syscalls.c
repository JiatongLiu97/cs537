#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int i;
  int pid = getpid();
  int total = atoi(argv[1]);
  int good = atoi(argv[2]);
  if(argc < 3){
    printf(2, "usage: syscalls totalsyscalls goodsyscalls\n");
    exit();
  }
  // make good sys calls
  for(i=1; i<good; i++) {
      getpid();
  }
  // make bad sys calls
  for(i = 1; i<(total-good)+1; i++) {
      close(2000);
  }
  int a = getnumsyscalls(pid);
  int b = getnumsyscallsgood(pid);

  printf(1, "%d %d\n", a, b);
  exit();
}
