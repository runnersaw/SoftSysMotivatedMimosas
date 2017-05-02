## GDB-Lite

### Authors
- Deniz Celik 
- Chong Swee Goh 
- Sawyer Vaughan 
- Chris Wallace


Our project is to create a program that replicates portions of [GDB](https://www.gnu.org/software/gdb/). GDB is a software program that allows developers to debug programs and give many pieces of useful information about the execution and state of the program.

### Compilation

`cd` into the `gdb-lite` directory and run the following make commands to compile or clean the project:

```bash
make package # Makes a gdb-lite executable in the gdb-lite directory
make clean # Removes temporary object files for a clean rebuild
```

### Usage
- To run GDB lite, compile an executable you wish to debug. Then run `./gdb-lite MY_EXEC`
- From here you can see the required input for each command you wish to run.


### Adding new code

This debugger will only work on 64 bit Linux. Modifying it to work for 32 bit Linux should be trivial, but it would be very difficult to get this debugger to work on a Mac. 

To add new code to the gdb-lite project, follow the directions below. Unless otherwise stated, assume all directories are relative to the gdb-lite/ directory (i.e. when it says src/ directory, it means gdb-lite/src/).

To add new code, write a source .c file in the src/ directory. Add a header as a .h file in the include/ directory that has declarations for the functions that need to be imported into `gdb-lite.c`.

Next, add `#include "filename.h"` to `gdb-lite.c`.

Next, we have to add all of the new files to the Makefile. Edit the src/Makefile file. Add `c_filename.o` to the `_OBJ` variable, and `h_filename.h` to the `_DEPS` variable.


