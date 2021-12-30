# P1: UNIX Utilities

## Overview
In this assignment, you will build a set of small utilities that are (mostly) versions or variants of commonly-used commands.  
We will call your version of these utilities  my-rev and my-look to differentiate them for the original utilities rev and look.

## Objectives
- Re-familiarize yourself with the C programming language
- Re-familiarize yourself with a shell / terminal / command-line of UNIX
- Learn (as a side effect) how to use a proper code editor such as emacs/vim
- Learn a little about how UNIX utilities are implemented
- See how pipes can be used to redirect stdout to stdin within a shell

While the project focuses upon writing simple C programs, you can see from the above list of objectives that this requires a great deal  of other previous knowledge, including a basic idea of what a shell is and how to use the command line on some UNIX-based systems (e.g., Linux or macOS), and how to use an editor.  If you do not have these skills already, or do not know C, this is not the right course to start learning these things.

This project is to be performed alone.  You can use up to three skip days throughout the semester for late projects; we recommend you do not use those skip days for this initial project.

Summary of what you will turn in:

- Set of .c files, one for each utility :  my-look.c, my-rev.c
- Each file should compile successfully when compiled with the -Wall and -Werror flags
- Each should pass tests we supply as well as tests that meet our specification that we do not supply
- A screenshot showing the output of my-rev piped as the input of my-look
