#include <stdio.h>
#include "malloc.h"

int main(int argc, char const *argv[])
{
    int *arr_1 = malloc(sizeof(int) * 10);
    for (int it = 0; it < 10; ++it) {
        arr_1[it] = it;
        printf("%i ", arr_1[it]);
    }
    free(arr_1);
    int *arr_2 = malloc(sizeof(int) * 10);
    for (int it = 0; it < 10; ++it) {
        arr_2[it] = it + 1;
        printf("%i ", arr_1[it]);
    }
    free(arr_2);
    return 0;
}
