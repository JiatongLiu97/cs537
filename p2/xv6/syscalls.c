#include "types.h"
#include "stat.h"
#include "user.h"



int
main(int argc, char **argv)
{

  if(argc != 3){
    printf(2, "usage: syscalls N g...\n");
    exit();
  }

  char str[10];
  int pid = getpid();
  getpid();
  write(1, str, 0);
  int i = getnumsyscalls(pid);
  int j = getnumsyscallsgood(pid);
  printf(1, "N = %d g = %d\n", i, j);
  exit();
}

