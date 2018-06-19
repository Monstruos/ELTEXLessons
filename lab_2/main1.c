#include <stdlib.h>


int main(int argc, char const *argv[])
{
    int *ptr = NULL;
    int i;
    for (i = 0; i < 10; ++i) {
        ptr = (int *) malloc(sizeof(int) * 1000);
    }
    
    free(ptr);
    return 0;
}
