#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
  char * str = "Going to segfault!\n";
  write(1, str, strlen(str));
  int * fault = 0x0;

  return *fault; 
}
