#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <unistd.h> 

#include "breakpoint.h"
int insertBreakpoint(int pid) {
  pid_t traced_process;
   int wait_status;

  struct user_regs_struct regs;
  long ins;

  //run objdump -d on the executable you want to find
  //Location of insturction to stop on
  unsigned breakAddress = 0x4005a2;
  traced_process = pid;
  wait(NULL);
  //GEt the contents of registers
  ptrace(PTRACE_GETREGS, traced_process,
         NULL, &regs);
  //find the instruction at the memory address
  ins = ptrace(PTRACE_PEEKTEXT, traced_process, 
    (void*)breakAddress, 0);
  //IDK why this is really being done, but it codes for the trap instruction
  unsigned data_with_trap = (ins & 0xffffff00) | 0xCC;
  //Write trap instruction
  ptrace(PTRACE_POKETEXT, traced_process, breakAddress, data_with_trap);
  printf ("Data now %08x \n", ptrace(PTRACE_PEEKTEXT, traced_process, breakAddress, 0));
  //Keep it going
  ptrace(PTRACE_CONT, traced_process, 0,0 );
  //wait(&wait_status);
  // if (WIFSTOPPED(wait_status)) {
  //   printf("Child got signal %s\n", strsignal(WSTOPSIG(wait_status)));

  // }
  //Check registers again
  ptrace(PTRACE_GETREGS, traced_process,
         NULL, &regs);
  //print location of instruction pointer, should be +1 of breakADdress
  printf("Child stopped at %08x\n", regs.rip);
  return 0;
}