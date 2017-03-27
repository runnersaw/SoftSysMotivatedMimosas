
# Security Project Proposal

##### Deniz Celik, Sawyer Vaughan, Chris Wallace, Chong Swee

## Project Goals
For this project we want to understand much more about how debuggers such as GDB work. While we will obviously not be able to create all of the functionality of GDB, we are interested in at least recreating some of that functionality

### Lower bound:
 - Create a breakpoint in a running c file
 - Find memory location of function pointers
 - Inspect memory at that breakpoint
 
### Upper Bound:
 - Inspect core dumps (include backtrace)
 - Step through code

## Research

We have done research and found information about the different capabilities of GDB:
 - GDB internals manual: https://sourceware.org/gdb/wiki/Internals
 - Ptrace man file: https://linux.die.net/man/2/ptrace
 - Tutorials on writing your own debuger: https://www.codeproject.com/Articles/1073879/Write-Your-Own-Linux-Debugger
 - More debugging info: http://www.alexonlinux.com/how-debugger-works
 - About the architecture of GDB: http://www.aosabook.org/en/gdb.html
 - Understanding C by learning assembly: https://www.recurse.com/blog/7-understanding-c-by-learning-assembly 
 - Overview of GDB Functionality https://www.recurse.com/blog/5-learning-c-with-gdb

## Project Backlog
Our project backlog is at the following URL: https://trello.com/b/aVbEVtPi/softsysmotivatedmimosas

## Potential Roadblocks
  Our primary roadblock will be finding the information that we need. In addition, there are many technical challenges with large parts of this. For example, it might be challenging to map from machine instruction to source code line, or to step through a program line by line, or to display the current assembly instruction. I also feel that we will be challenged in ensuring that this will work on different operating systems (because this information may be architecture dependent, we will probably have to ensure we are on the same operating systems and specify that our program only works on certain operating systems.We plan on using developing on Ubuntu 14.04, either with VM’s or dual boots in order to standardize the development environment.


