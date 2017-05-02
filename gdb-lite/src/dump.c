#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <unistd.h>
#include <string.h>

#include "breakpoint.h"
#include "dump.h"

//function for printing of registers
#define printreg(REGNAME) printf("%3s: 0x%016llx\n", #REGNAME, regs.REGNAME)

struct user_regs_struct regs;

/*
  Function:  Prints contents of stack
  Input:     Breakpoint (Structure)
*/
void printStack(Breakpoint* breakpoint){

  unsigned long addr = breakpoint->address;
  int child_pid = breakpoint->pid;

  // gets register info
  ptrace(PTRACE_GETREGS, child_pid, 0, &regs);

  // prints base pointer and stack pointer
  printreg(rsp);
  printreg(rbp);

  //Checks if the stack is empty
  if (regs.rbp <= regs.rsp){
    fprintf(stderr,"Error in dumping stack, invalid breakpoint?\n");
    return;
  }

  int i = 0;
  void* rsp_addr = (void*)((unsigned long int)(regs.rsp + i));

  // loops and print the stack (4 Bytes at a time)
  while((unsigned long int)rsp_addr!= regs.rbp){
    long rsp_val = ptrace(PTRACE_PEEKDATA, child_pid, rsp_addr, NULL);

    if(rsp_val != -1) {
      printf("(Address:0x%016lx), Value(32 bits): %10ld or 0x%08lx\n", (unsigned long int) rsp_addr, rsp_val &0xFFFFFFFF, rsp_val&0xFFFFFFFF);
    } else {
      printf("Error in reading register : 0x%016lx",(unsigned long int) rsp_addr);
    }
    i += 4; // I think each address stores a byte? , printing each 32 bits
    rsp_addr = (void*)((unsigned long int)(regs.rsp + i));
  }
}
