#include <stdlib.h>
#include <stdio.h>


int main(int argc, char const *argv[])
{
    int *ptr = NULL;
    int i, a;
    int b;
    ptr = (int *) malloc(sizeof(int) * 1000);
    printf("[");
    for (i = 0; i < 10; ++i) {
        printf("%i ", ptr[1000 + i]);
    }
    printf("\b] ");
    printf("a = %i, ", a);
    printf("b = %i \n", b);
    free(ptr);
    return 0;
}
