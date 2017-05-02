/*
This file is responsible for handling breakpoint functionality within our debugger.
This file provides methods capable of inserting a breakpoint into a file and resuming 
the previously created breakpoint.
*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <unistd.h> 
#include <string.h>

#include "breakpoint.h"

/*
Constructor for the Breakpoint object. This objecct will be populated
with the previous instruction, the memory address it is inserted at
and the PID of the process it is attached to.
*/
Breakpoint *make_breakpoint() {
    Breakpoint *breakpoint = malloc(sizeof(Breakpoint));
    return breakpoint;
}

int wait_status;
struct user_regs_struct regs;

/*
Inserts a breakpoint onto the PID specified by breakpoint->pid
at the address given by breakpoint->address.
*/
void insertBreakpoint(Breakpoint *breakpoint) {
    
    unsigned long addr = breakpoint->address;
    int child_pid = breakpoint->pid;
    
    printf("debugger started\n");

    // Wait for child to stop on its first instruction
    wait(&wait_status);

    // Obtain and show child's instruction pointer
    ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
    printf("Child started. Instruction pointer = 0x%08llx\n", regs.rip);
    
    breakpoint->previousInstruction = ptrace(PTRACE_PEEKTEXT, child_pid, (void*)addr, 0);
    printf("Original data at 0x%08lx: 0x%08lx\n", addr, breakpoint->previousInstruction);

    // Write the trap instruction 'int 3' into the address
    unsigned long data_with_trap = (breakpoint->previousInstruction & 0xFFFFFFFFFFFFFF00) | 0xCC;
    ptrace(PTRACE_POKETEXT, child_pid, (void*)addr, (void*)data_with_trap);

    // See what's there again...
    unsigned long readback_data = ptrace(PTRACE_PEEKTEXT, child_pid, (void*)addr, 0);
    printf("After trap, data at 0x%08lx: 0x%08lx\n", addr, readback_data);

    // Let the child run to the breakpoint and wait for it to reach it
    ptrace(PTRACE_CONT, child_pid, 0, 0);

    wait(&wait_status);
    if (WIFSTOPPED(wait_status)) {
        printf("Child got a signal: %s\n", strsignal(WSTOPSIG(wait_status)));
    } else {
        perror("wait");
        return;
    }

    // See where the child is now
    ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
    printf("Child stopped. Instruction pointer = 0x%08llx\n", regs.rip);
}
/*
 * Remove the breakpoint by restoring the previous data
 * at the target address, and unwind the EIP back by 1 to
 * let the CPU execute the original instruction that was
 * there.
 */
void resumeBreakpoint(Breakpoint *breakpoint) {

    unsigned long addr = breakpoint->address;
    unsigned long data = breakpoint->previousInstruction;
    int child_pid = breakpoint->pid;
    ptrace(PTRACE_POKETEXT, child_pid, (void*)addr, (void*)data);
    regs.rip -= 1;
    ptrace(PTRACE_SETREGS, child_pid, 0, &regs);

    // The child can continue running now
    ptrace(PTRACE_CONT, child_pid, 0, 0);

    wait(&wait_status);

    if (WIFEXITED(wait_status)) {
        printf("Child exited\n");
    } else {
        printf("Unexpected signal\n");
        printf("Yes: %s \n", strsignal(WSTOPSIG(wait_status)));
    }
}

