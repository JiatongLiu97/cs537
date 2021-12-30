# P3: Scheduling

## Overview
In this project, you’ll be modifying the Round Robin (RR) CPU scheduler in xv6 so that processes can have different time-slice lengths and are "compensated" in different ways for the amount of time they are blocked (and thus cannot be scheduled).  

## Objectives
- To understand existing code for performing context-switches in the xv6 kernel
- To implement a basic round-robin (RR) scheduler that compensates processes that relinquish the CPU
- To implement system calls that extract process states
- To implement a user-level program that tests the basic compensation behavior of the scheduler

## Implementation
