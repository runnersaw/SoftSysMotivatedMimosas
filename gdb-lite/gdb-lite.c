
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/reg.h>
#include <sys/ptrace.h>

#include "run.h"
#include "breakpoint.h"

#define MAX_INPUT_LENGTH 100

int IS_AT_BREAKPOINT = 0;

int main(int argc, char *argv[]) {
  unsigned long previousBreakpointInstruction;
  unsigned long addr;
  int pid;
  Breakpoint *breakpoint = make_breakpoint();
  char *input = malloc((MAX_INPUT_LENGTH + 1) * sizeof(char));
  input[MAX_INPUT_LENGTH] = '\0';

  if (argc <= 1) {
    printf("Usage: gdb-lite program_name\n");
    return -1;
  }

  char *fname = argv[1];

  while (fgets(input, MAX_INPUT_LENGTH, stdin) != NULL) {
    // Interpret command here
    if (strcmp(input, "run\n") == 0) {

        //If it is the child then we wanna run this
        int code = run(fname);
        printf("%s exited with code %d\n", fname, code);
      
      
    } else if (strcmp(input, "break\n") == 0) {
      printf("Please enter address to insert breakpoint on\n");
      fgets(input, MAX_INPUT_LENGTH, stdin);
      //0x4005a2 is the address I use for test
      addr = strtol(input, NULL, 0);

      IS_AT_BREAKPOINT = 1;

      pid = fork();
      if (pid == 0) {
        //If it is the child then we wanna run this
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        int code = execl(fname, fname, 0);
        printf("%s exited with code %d\n", fname, code);
      } else {
        breakpoint->address = addr;
        breakpoint->pid = pid;
        insertBreakpoint(breakpoint);
        //run_debugger(pid);
        //We are the parent we want to attach a process to this PID
        
      }
    } else if(strcmp(input, "resume\n") == 0) {
      if (!IS_AT_BREAKPOINT) {
        printf("Code is not at breakpoint, nothing to resume\n");
      }
      else {
        resumeBreakpoint(breakpoint );
      }
    }
    else {
      printf("Command not handled: %s\n", input);
    }
  }

  return 0;
}


