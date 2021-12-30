# P2 - A: xv6 New System Call

## Overview
This first project is intended to be a warmup, and thus relatively light. You will not write many lines of code for this project. 

Instead, a lot of your time will be spent learning where different routines are located in the existing source code. 

## Objectives
- Gain comfort looking through more substantial code bases written by others in which you do not need to understand every line
- Obtain familiarity with the xv6 code base in particular
- Learn how to add a system call to xv6 
- Add a user-level application that can be used within xv6
- Become familiar with a few of the data structures in xv6 (e.g., process table)
- Use the gdb debugger on xv6 

## Implementation
System Calls and User-Level Application
You will add two related system calls to xv6:

- int getnumsyscalls(int pid)  returns the number of system calls that the process identified by pid has completed (not just initiated); calls to getnumsyscalls(), getnumsyscallsgood(), fork(), exec(), and sbrk() should not be counted in that total*. Returns -1 if pid is not a valid pid.
- int getnumsyscallsgood(int pid)  returns the number of system calls that the process identified by pid has completed successfully (i.e., with a return code that is not -1); calls to getnumsyscalls(), getnumsyscallsgood(), fork(), exec(), and sbrk() should not be counted in this total. Returns -1 if pid is not a valid pid.
So that you can test your system calls, you should also create a user-level application with the following behavior:

- syscalls N g. This program takes two arguments: N, which is the total number of system calls that it should make, and g, which is the number of those which should be successful. Note that N and g should >= 1 because you always need a syscall getpid() to know the pid of the current running process. You can choose to make any system calls that you know will be successful or unsuccessful; you can perform those system calls in any order you choose. After this work has been done, it should print out two values: the value returned by getnumsyscalls(mypid) and the value returned by getnumsyscallsgood(mypid). For example, if you run the program as "syscalls 20 5" the output should be exactly "20 5\n". You can handle errors (e.g., an incorrect number of arguments or the number of good system calls is greater than the total number of calls however you choose) but we won't test it. 
You must use the names of the system call and the application exactly as specified!

*Note:

- Typically, fork(), exec(), and sbrk() will be called during the process creation (before the main() of the user-level application syscalls  is invoked); for simplicity, we exclude them here. You should also be careful when calling some library functions as they may also invoke syscalls (e.g. malloc() might call sbrk()). Try to avoid them before printing the final result.
- A valid pid indicates any pid that is in-use which includes the zombie processes. A process is in the zombie state after it exits and before it is waited for by its parent process.
