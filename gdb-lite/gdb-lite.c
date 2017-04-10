
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "run.h"

#define MAX_INPUT_LENGTH 1000

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

char *rest_of_string_after_delimiter(char *input, char *delimiter) {
  char *res = strstr(input, delimiter);
  if (res == NULL) {
    return "";
  }
  char *output = res + strlen(delimiter);
  return output;
}

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

int main(int argc, char *argv[]) {
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
    } else {
      printf("Command not handled: %s\n", input);
    }
  }

  return 0;
}


