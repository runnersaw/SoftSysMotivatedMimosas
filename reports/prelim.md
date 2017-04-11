
# GDB-Lite Preliminary Proposal

##### Deniz Celik, Sawyer Vaughan, Chris Wallace, Chong Swee

## Project Goals
For this project we want to understand much more about how debuggers such as GDB work. While we will obviously not be able to create all of the functionality of GDB, we are interested in at least recreating some of that functionality.

### Lower bound:
 - Create a breakpoint in a running c file
 - Find memory location of function pointers
 - Inspect memory at that breakpoint
 
### Upper Bound:
 - Inspect core dumps (include backtrace)
 - Step through code
 - Obtain value of a variable during or after execution
 
 ## Progress
 We have completed the wollowing trello cards
 - Read Architecture of GDB
    - We will refine done criteria in the future to more effectively handle research oriented tasks.
- Write REPL code for interfacing with our functions
  - [Code](https://github.com/runnersaw/SoftSysMotivatedMimosas/blob/master/gdb-lite/gdb-lite.c)
- Download and compile BFD utils for reading obj files
- Create function to dump memory around an address
  - [Code](https://github.com/runnersaw/SoftSysMotivatedMimosas/blob/master/gdb-lite/src/printadd.c)
- Create a Makefile for easy compilation of the project
  - [Makefile](https://github.com/runnersaw/SoftSysMotivatedMimosas/blob/master/gdb-lite/Makefile)
- Create program showing basic functionality of ptrace. 
  - [Code](https://github.com/runnersaw/SoftSysMotivatedMimosas/blob/master/gdb-lite/src/trace.c)

Below is a screenshot of the output of our program so far.
![Output](https://github.com/runnersaw/SoftSysMotivatedMimosas/blob/master/reports/Output.png)

## Resources
We have compiled the following set of helpful resources that will help us to complete this project successfully.
- [GDB internals manual](https://sourceware.org/gdb/wiki/Internals)
  - A useful but very technical description of the internals of GDB.
- [Ptrace man file](https://linux.die.net/man/2/ptrace)
  - The documentation of ptrace, which will be needed for us to debug other processes. GDB makes heavy use of ptrace for large portions of its functionality.
- [Tutorials on writing your own debugger](https://www.codeproject.com/Articles/1073879/Write-Your-Own-Linux-Debugger)
  - This is a useful resource of someone who has attempted to do something similar to what we are currently doing.
- [About the architecture of GDB](http://www.aosabook.org/en/gdb.html)
  -A high level overview of the main functions of GDB and the underlying mechanisms for many critical GDB functions.
- [Overview of GDB Functionality](https://www.recurse.com/blog/5-learning-c-with-gdb)
- [Ptrace breakpoints tutorial](http://eli.thegreenplace.net/2011/02/07/how-debuggers-work-part-3-debugging-information)
  - Tutorial documenting how breakpoints work in C. It works incrementally from manually changing assembly commands to supplying a line of c code and inserting a breakpoint there. This tutorial introduced me to the DWARF debugging standard. The unfortunate thing about this is that it is written for i386 and many of the specific C library calls for Ptrace have changed in x64.  However, the logic still holds. 

## Project Backlog
Our project backlog is at the following URL: https://trello.com/b/aVbEVtPi/softsysmotivatedmimosas

## Potential Roadblocks
The largest potential roadblock is ourselves. Senioritis has hit hard this past sprint, along with beautiful weather and lawn chairs. We hope to alleviate this by locking ourselves inside to work. All jokes aside, we have a few small roadblocks in terms of the amount of time left in the sprint to complete our goals, but with the resources available above and online, we are sure we can at least meet our minimum deliverables. As of now, we don’t believe we will need any extra materials nor do we have any pressing questions .
