
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/reg.h>
#include <sys/ptrace.h>

#include "run.h"
#include "breakpoint.h"
#include "delimiter.h"
#include "dump.h"


#define MAX_INPUT_LENGTH 1000

/*
get_input
  Takes an already malloc'ed char * and will copy the input from the user into it.
  The `input` argument should be an already malloc'ed string with at least MAX_INPUT_LENGTH
  of space.

  This is essentially a wrapper around fgets that prints "> " before user input, and cleans
  the newline (\n) from the end of the string.
*/
char *get_input(char *input) {
  printf("> ");
  char *ret = fgets(input, MAX_INPUT_LENGTH, stdin);
  if (ret == NULL) {
    return ret;
  }

  // Clean newline
  int len = strlen(input) -1;
  if (input[len] == '\n') {
    input[len] = '\0';
  }

  // Man page for fgets says it returns input string, so let's do that
  return input;
}

int IS_AT_BREAKPOINT = 0;

int main(int argc, char *argv[]) {
  int pid;
  Breakpoint *breakpoint = make_breakpoint();
  char *input = malloc((MAX_INPUT_LENGTH + 1) * sizeof(char));
  input[MAX_INPUT_LENGTH] = '\0';

  if (argc <= 1) {
    printf("Usage: gdb-lite program_name\n");
    return -1;
  }

  char *fname = argv[1];

  while (get_input(input) != NULL) {
    // Split on whitespace
    char **args = split_on_delimiter(input, " \n");
    if (*args == NULL) {
      perror("Couldn't find input arguments");
      free(args);
      continue;
    }
    char *command = *args;

    // Interpret command here
    if (strcmp(command, "run") == 0) {
      int code = run(fname, args);
      printf("\n%s exited with code %d\n", fname, code);
    } else if (strcmp(command, "quit") == 0) {
      return 0;
    } else if (strcmp(command, "break") == 0) {
      printf("Please enter address to insert breakpoint on\n");
      fgets(input, MAX_INPUT_LENGTH, stdin);
      //0x4005a2 is the address I use for test
      unsigned long addr = strtol(input, NULL, 0);

      IS_AT_BREAKPOINT = 1;

      pid = fork();
      if (pid == 0) {
        //If it is the child then we wanna run this
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        int code = execl(fname, fname, (char *)NULL);
        printf("%s exited with code %d\n", fname, code);
      } else {
        breakpoint->address = addr;
        breakpoint->pid = pid;
        insertBreakpoint(breakpoint);
        //run_debugger(pid);
        //We are the parent we want to attach a process to this PID

      }
    } else if(strcmp(command, "resume") == 0) {
      if (!IS_AT_BREAKPOINT) {
        printf("Code is not at breakpoint, nothing to resume\n");
      }
      else {
        resumeBreakpoint(breakpoint);
      }
    } else if (strcmp(command,"dump") == 0) {
      if (!IS_AT_BREAKPOINT) {
        printf("Code is not at breakpoint, nothing to dump\n");
      }
      else{
        printStack(breakpoint);
      }
    } else if (strcmp(command,"print") == 0) {
      printf("Please enter name of function to print\n");
      fgets(input, MAX_INPUT_LENGTH, stdin);
      int code = print_function_symbol(fname, strtok(input,"\n"));
      if (code == 0) {
        printf("%s was not found in file %s", input, fname);
      }
    } else if (strcmp(command,"symtable") == 0) {
      print_symbol_table(fname);
    } else {
      printf("Command not handled: %s\n", input);
    }

    // Free the args strings that we copied
    free_strings(args);
    free(args);
  }

  return 0;
}
