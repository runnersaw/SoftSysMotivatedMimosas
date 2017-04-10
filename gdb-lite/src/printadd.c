#include<stdio.h>
#include<stdlib.h>

/*
    Input:
      1.int pointer
      2.int k (Number of address to print after pointer's address)
    Output: Prints address and its contents
*/
void print_add(int *ptr, int k){
  printf("\nPRINTING ADDRESSES \n");
  int i=0;
  while(i<k){
    printf("Address: %p Content %d\n",(ptr+i),*(ptr+i));
    i++;
  }
  return;
}

int main(){

  int q = 10;
  int o = 20;
  print_add(&q, 10);

}
