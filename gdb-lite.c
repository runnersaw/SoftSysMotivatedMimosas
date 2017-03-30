
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_INPUT_LENGTH 100

int main(int argc, char *argv[]) {
  char *input = malloc((MAX_INPUT_LENGTH + 1) * sizeof(char));
  input[MAX_INPUT_LENGTH] = '\0';

  if (argc <= 1) {
    printf("Usage: gdb-lite program_name\n");
    return -1;
  }

  char *fname = argv[1];

  // Check if we have execute permission
  if(access(fname, F_OK) != -1) {
    // file exists
    if (access(fname, X_OK) != -1) {
      // TODO: security checks?
      system(fname);
    } else {
      printf("No executable permission for %s", fname);
      return -1;
    }
  } else {
    // file doesn't exist
    printf("File %s doesn't exist\n", fname);
    return -1;
  }

  while (fgets(input, MAX_INPUT_LENGTH, stdin) != NULL) {
    // Interpret command here

  }

  return 0;
}


