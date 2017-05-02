
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "run.h"
#include "delimiter.h"

int run(char *fname, char **args) {
  // Check if we have execute permission
  if(access(fname, F_OK) != -1) {
    // file exists
    if (access(fname, X_OK) != -1) {
      // TODO: security checks?

      printf("Running command:\n");
      printf("%s ", fname);
      print_strings(args, " ", 1);
      return execv(fname, args);
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



