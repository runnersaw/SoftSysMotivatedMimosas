
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/reg.h>
#include <sys/ptrace.h>

#include "run.h"
#include "breakpoint.h"

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

/*
rest_of_string_after_delimiter
  Takes an input string and a delimiter, and returns a newly allocated string that
  contains the portion of the input string that is after the first instance of the
  delimiter.

  It is your responsibility to free the returned string!
*/
char *rest_of_string_after_delimiter(char *input, char *delimiter) {
  char *res = strstr(input, delimiter);
  if (res == NULL) {
    return "";
  }

  res = res + strlen(delimiter);

  int len = strlen(res) + 1;
  char *output = malloc(len*sizeof(char));
  strcpy(output, res);
  output[len-1] = '\0';

  return output;
}

/*
first_string_before_delimiter
  Takes an input string and a delimiter, and returns a newly allocated string that
  contains the portion of the input string that is before the first instance of the
  delimiter.

  It is your responsibility to free the returned string!
*/
char *first_string_before_delimiter(char *input, char *delimiter) {
  // Find first instance of delimiter
  char *res = strstr(input, delimiter);

  int len;
  // If not found, copy whole string. Else, the length we need to copy is the difference between the two.
  if (res == NULL) {
    len = strlen(input);
  } else {
    len = (res - input) / sizeof(char);
  }
  char *output = malloc((len + 1) * sizeof(char));
  int i;
  for (i = 0; i < len; i++) {
    output[i] = input[i];
  }
  output[len] = '\0';
  return output;
}

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

  while (get_input(input) != NULL) {
    // Split on whitespace
    char *command = first_string_before_delimiter(input, " ");

    // Interpret command here
    if (strcmp(command, "run") == 0) {
      char *args = rest_of_string_after_delimiter(input, " ");

      int code = run(fname, args);
      printf("\n%s exited with code %d\n", fname, code);
    } else if (strcmp(command, "quit") == 0) {
      return 0;
    } else if (strcmp(command, "break") == 0) {
      printf("Please enter address to insert breakpoint on\n");
      fgets(input, MAX_INPUT_LENGTH, stdin);
      //0x4005a2 is the address I use for test
      addr = strtol(input, NULL, 0);

      IS_AT_BREAKPOINT = 1;

      pid = fork();
      if (pid == 0) {
        //If it is the child then we wanna run this
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        int code = execl(fname, fname, (char *)0);
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
   } else
      printf("Command not handled: %s\n", input);
  }

  return 0;
}
