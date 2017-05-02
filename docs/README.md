## Goal

For our final Software Systems Project we created a C debugger modeled after GDB. Our debugger allows us to identify the memory location of user defined functions, set breakpoints in code and inspect the content of addresses in memory. 

## Project Motivation

The debugger is an important tool that programmers should learn about and be familiar with in order to quickly debug problems with their programs or inspect program functionality closely. GDB is a useful tool, especially in a low-level language like C, where errors like mishandling memory are too easy and information about the error isn’t clear.

We wanted to replicate this functionality in order to learn about how debuggers work, as well as to learn about how processes can inspect, alter, and control other processes. Modifying executables has many interesting applications, and we wanted to learn about how this was done on Linux.

GDB is a very complicated tool and includes a lot of functionality that even advanced GDB users rarely touch. We wanted to replicate the important pieces of GDB functionality.

## Project Scope

Debuggers like GDB have a huge range of functionality. Implementing all of it would be outside of the scope of our project. In order to determine what functionality we wanted to implement we decided to start by determining what functionality we found to be the most useful while writing C code. We realized that the most important functionality all was built on top of the ability to set breakpoints. We frequently found that we would use debuggers to view memory after a certain point in our program or to step through code to see how memory changes. We determined this was the core piece of functionality that we wanted to implement within our project. On top of that, we realized that breakpoints are most useful if we know the memory address of functions we wrote. This was we can set breakpoints when we call functions without having to dig through assembly. This led us to add our second big piece of functionality which was the ability to find the memory address of a user defined function in the symbols table.

What we found most compelling about this project is that it works at a very low level. We frequently have to look at assembly code and understand what is going on at the lowest level of C in order to get our debugger to work.

## Functionality

Our implementation starts with a REPL loop. Once within this REPL loop users can input different commands to see the functionality of our debugger. Each command has slightly different architectural requirements to get them to function, but "run" and "breakpoint" operate very similarly. They both fork off a child process that runs the executable desired. Once there the parent script stays in the REPL loop allowing users to do perform additional operations on the executable. The following commands are available:

- run: run the provided program with any arguments required to run it
- quit: quit the REPL loop
- break: ask the user to provide an address to insert a breakpoint at
- resume: resume a program that has reached a breakpoint (can only be used if the program has reached a breakpoint)
- dump: dump the contents of the stack (can only be used if the program has reached a breakpoint)
- print: prints out the symbol table entry of a provided function
- symtable: prints out the entire symbol table

### ptrace

Much of the functionality of GDB is implemented using ptrace. The ptrace functions available on Linux allows you to attach to a process and view and modify memory, registers, and execution of that process. It makes up the backbone of debuggers like GDB. In our program, the breakpoint functionality relies on the function ptrace. ptrace uses a lot of architecture specific code. As a result, our code only works on 64 bit Linux systems. It will not work on Mac or Windows.

### Symbols

Using the linux nm implementation and other online resources as references, we developed a simplified version to be able to search for function names and print their addresses in addition to printing the entire symbol table. This allows users to find addresses using gdb-lite instead of having to use other unix utilities.

