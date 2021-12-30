# P2 - B: Shell

## Overview
Make sure you read this entire specification before beginning.

In this assignment, you will implement a command line interpreter, or shell, on top of Linux. The shell should operate in this basic way: when you type in a command (in response to its prompt), the shell creates a child process that executes the command you entered and then prompts for more user input when it has finished.  More specifically, shells are typically implemented as a simple loop that waits for input and 'fork()'s a new child process to execute the command; the child process then 'exec()'s the specified command while the parent process 'wait()'s for the child to finish before continuing with the next iteration of the loop.

The shell you implement will be similar to, but much simpler, than the one you run every day in Unix. You can find out which shell you are running by typing 'echo $SHELL' at a prompt. You may then wish to look at the man pages for 'sh' or the shell you are running to learn more about all of the functionality that can be present. For this project, you do not need to implement as much functionality as is in most shells. 

Besides the most basic function of executing commands, your shell ('mysh') must provide the following three features: interactive vs. batch mode, output redirection, and aliasing.

## Objectives
- To gain yet more familiarity with programming in C and in Linux
- To learn how processes are handled with fork(), exec(), and wait()
- To gain exposure to some of the functionality in shells
- To understand how redirection of stdout works

## Features
2.1) Modes: Interactive and Batch
Your shell can be run in two modes: interactive and batch.   The mode is determined when your shell is started.  If your shell is started with no arguments (i.e., ./mysh) , it will run in interactive mode; if your shell is given the name of a file (e.g., ./mysh batch-file), it runs in batch mode.

In interactive mode, you will display a prompt (the string mysh> , note the space AFTER the > character) to stdout) and the user of the shell will type in a command at the prompt.

In batch mode, your shell is started by specifying a batch file on its command line; the batch file contains the list of commands (each on its own line) that should be executed. In batch mode, you should not display a prompt. In batch mode, you should echo each line you read from the batch file back to the user (stdout) before executing it; this will help you when you debug your shells (and us when we test your shell -- most of which will be of batch mode). If the line is empty or only composed of whitespace, you should still echo it; if it is over the 512-character limit then echo at least the first 512 characters (but you may echo more if you want).

In both interactive and batch mode, your shell terminates when it sees the exit command on a line or reaches the end of the input stream (i.e., the end of the batch file or the user types 'Ctrl-D').  

2.2) Redirection
Many times, a shell user prefers to send the output of a program to a file rather than to the screen. Usually, a shell redirects standout output to a file with the > character; your shell should include this feature.

For example, if a user types /bin/ls -la /tmp > output into your shell, nothing should be printed on the screen.  Instead, the standard output of the ls program should be rerouted to the file output.  Note that standard error output should not be changed; it should continue to print to the screen.  If the output file exists before you run your program, you should simply overwrite it (after truncating it, which sets the file's size to zero bytes).

The exact format of redirection is: a command (along with its arguments, if present), followed by any number of white spaces (including none), the redirection symbol >,  again any number of white space (including none), followed by a filename.

Special cases: Multiple redirection operators (e.g. /bin/ls > > file.txt ), starting with a redirection sign (e.g. > file.txt ), multiple files to the right of the redirection sign (e.g. /bin/ls > file1.txt file2.txt ), or not specifying an output file (e.g. /bin/ls > )are all errors.  Print exactly: Redirection misformatted.\n.  If if the output file cannot be opened for some reason (e.g., the user doesn't have write permission or the name is an existing directory), your shell should print exactly Cannot write to file foo.txt.\n.  In these cases, do not execute the command and continue to the next line.

Do not worry about redirection for built-in commands (alias, unalias, and exit); we will not test these cases.
