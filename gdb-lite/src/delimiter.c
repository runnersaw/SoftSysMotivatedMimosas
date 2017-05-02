
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
num_words_with_delimiter
  Takes an input string and returns the number of words that
  it contains that are separated by any character in delimiters.

  This is a utility function for split_on_delimiter.
*/
int num_words_with_delimiter(char *input, char *delimiters) {
  // Copy the input, it will be modified by strtok
  char *input_copy = strdup(input);

  // Keep track of current amount of words put into the output array.
  int count = 0;

  char *word = strtok(input_copy, delimiters);
  while (word != NULL)
  {
    word = strtok(NULL, delimiters);
    count++;
  }

  free(input_copy);
  return count;
}

/*
split_on_delimiter
  Takes an input string and a delimiter, an array (pointer to pointers) of
  strings. The last pointer in the array will be NULL.

  It is your responsibility to free the returned strings!
*/
char **split_on_delimiter(char *input, char *delimiters) {
  // Create the output with size 1. We will increase its size later
  int num_words = num_words_with_delimiter(input, delimiters);
  char **output = (char **)malloc((num_words + 1)*sizeof(char **));

  // Copy the input, it will be modified by strtok
  char *input_copy = strdup(input);

  // Keep track of current amount of words put into the output array.
  int count = 0;

  char *word = strtok(input_copy, delimiters);
  while (word != NULL)
  {
    if (strcmp(word, "") == 0) {
      word = NULL;
      continue;
    }
    // Copy the found word into the output array, then add another block at the end
    *(output + count) = strdup(word);
    count++;

    word = strtok(NULL, delimiters);
  }

  *(output + count) = (char *)NULL;

  free(input_copy);
  return output;
}

/*
free_strings
  Takes an pointer to (char *) and will free all strings that
  the array pointer points to.
*/
void free_strings(char **strings) {
  int count = 0;
  while (*(strings + count) != NULL) {
    free(*(strings + count));
    count++;
  }
}

/*
print_strings
  Prints all of the strings in the array, separated by the delimiter.
*/
void print_strings(char **strings, char *delimiter, int start) {
  int count = 0;
  while (*(strings + count) != NULL) {
    if (count >= start) {
      printf("%s%s", *(strings + count), delimiter);
    }
    count++;
  }
  printf("\n");
}
