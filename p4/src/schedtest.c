#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"
#include "pstat.h"
#include "syscall.h"


int main(int argc, char **argv){
    struct pstat pst;
    int compticksA = -1;
    int compticksB = -1;
    if(argc != 6) {
      printf(2, "6 arguments are needed including program name.\n");
      exit();
    }
    int sliceA = atoi(argv[1]);
    int sliceB = atoi(argv[3]);
    int slp = atoi(argv[5]);

    int pid1 = fork2(sliceA);
    if (pid1 == 0) {
      char *commands[3] = {"loop",argv[2],'\0'};
      exec("loop", commands);
    }
    int pid2 = fork2(sliceB);
    if (pid2 == 0) {
      char *commands[3] = {"loop",argv[4],'\0'};
      exec("loop", commands);
    }
    sleep(slp);
    if (getpinfo(&pst) == 0) {
      for (int i = 0 ; i < NPROC; i++){
          if (pst.pid[i] == pid1) 
            compticksA = pst.compticks[i];
          if (pst.pid[i] == pid2) 
            compticksB = pst.compticks[i];
        }
      
    }
    printf(1, "%d %d\n", compticksA, compticksB);
    wait();
    wait();
    exit();
    
}