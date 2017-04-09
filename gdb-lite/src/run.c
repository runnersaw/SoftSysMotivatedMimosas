
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "run.h"

int run(char *fname) {
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

}



