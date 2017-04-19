
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "run.h"
#include "breakpoint.h"

#define MAX_INPUT_LENGTH 100

int main(int argc, char *argv[]) {
  int pid;
  char *input = malloc((MAX_INPUT_LENGTH + 1) * sizeof(char));
  input[MAX_INPUT_LENGTH] = '\0';

  if (argc <= 1) {
    printf("Usage: gdb-lite program_name\n");
    return -1;
  }

  char *fname = argv[1];

  while (fgets(input, MAX_INPUT_LENGTH, stdin) != NULL) {
    // Interpret command here
    if (strcmp(input, "run") == 0) {
      pid = fork();

      if (pid == 0) {
        //If it is the child then we wanna run this
        int code = run(fname);
        printf("%s exited with code %d\n", fname, code);
      } else {
        insertBreakpoint(pid);
        //We are the parent we want to attach a process to this PID
        
      }
      
      
    } else {
      printf("Command not handled: %s\n", input);
    }
  }

  return 0;
}


