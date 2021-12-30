#include "types.h"
#include "stat.h"
#include "user.h"


int main(int argc, char **argv){
    if (argc != 2)   
      exit();
    
    
    int sleepT = atoi(argv[1]);
    
    if (sleepT < 1) {
      printf(1, "timeslice should be positive.\n");
      exit();
    }
    
    sleep(sleepT);
    
    
    int i = 0, j = 0;
    while (i < 800000000) {
      j += i * j + 1;
      i++;
    }
    
    exit();
}
