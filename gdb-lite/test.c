#include <stdlib.h>
#include <stdio.h>

int i = 10;

int main()
{
	int j = 2;
    for(i = 0;i < 10; ++i) {
        printf("My counter: %d\n", i);
        sleep(2);
    }
    printf("Printing j: %d\n", j);
    return 0;
}
