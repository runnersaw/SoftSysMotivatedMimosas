#include <stdlib.h>
#include <stdio.h>

int i = 0;

int main()
{
    for(i = 0;i < 10; ++i) {
        printf("My counter: %d\n", i);
        sleep(2);
    }
    return 0;
}
