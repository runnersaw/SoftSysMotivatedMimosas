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

void printStack(Breakpoint* breakpoint){
  int child_pid = breakpoint->pid;
  ptrace(PTRACE_GETREGS, child_pid, 0, &regs);

  printreg(rsp);
  printreg(rbp);

  if (regs.rbp < regs.rsp){
    fprintf(stderr,"Error in dumping stack, invalid breakpoint?\n");
    return;
  }

  if (regs.rbp == regs.rsp){
    fprintf(stdout,"Nothing on the stack to dump.\n");
    return;
  }

  int i = 0;
  void* rsp_addr = (void*)((unsigned long int)(regs.rsp + i));

  while((unsigned long int)rsp_addr!= regs.rbp){
    long rsp_val = ptrace(PTRACE_PEEKDATA, child_pid, rsp_addr, NULL);

    if(rsp_val != -1) {
      printf("(Address:0x%016lx), Value(32 bits): %16ld or 0x%08lx\n", (unsigned long int) rsp_addr, rsp_val, rsp_val);
    } else {
      printf("Error in reading register : 0x%016lx",(unsigned long int) rsp_addr);
    }
    i += 4; // I think each address stores a byte? , printing each 32 bits
    rsp_addr = (void*)((unsigned long int)(regs.rsp + i));
  }
}
