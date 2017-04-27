
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "run.h"

int run(char *fname, char *args) {
  // Check if we have execute permission
  if(access(fname, F_OK) != -1) {
    // file exists
    if (access(fname, X_OK) != -1) {
      // TODO: security checks?

      // Length is filename length + space(1) + length of args + \0(1)
      int command_len = strlen(fname) + strlen(args) + 2;
      char *system_call = malloc(command_len*sizeof(char));
      int size = sprintf(system_call, "%s %s", fname, args);

      printf("Running command:\n");
      printf("%s\n\n", system_call);
      return system(system_call);
    } else {
      printf("No executable permission for %s", fname);
      return -1;
    }
  } else {
    // file doesn't exist
    printf("File %s doesn't exist\n", fname);
    return -1;
  }
}



