
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_LENGTH 100

int main(int argc, char *argv[]) {
  char *input = malloc((MAX_INPUT_LENGTH + 1) * sizeof(char));
  input[MAX_INPUT_LENGTH] = '\0';

  if (argc <= 1) {
    printf("Usage: gdb-lite program_name\n");
    return -1;
  }
  
  while (fgets(input, MAX_INPUT_LENGTH, stdin) != NULL) {
    // Interpret command here
    
  } 
  
  return 0;
}


