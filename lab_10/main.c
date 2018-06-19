#include <stdio.h>
#include "pack.h"

int main(int argc, char const *argv[]) {
    char str[10] = {'a', 0, 0, 0, 0,
                    'b', 0, 0, 0, 0};
    struct nonpack *np_ptr = (struct nonpack *)str;
    struct pack *p_ptr = (struct pack *)str;

    printf("non-packed: char a = %c, int b = %i\n", np_ptr->a, np_ptr->b);
    printf("packed: char a = %c, int b = %i\n", p_ptr->a, p_ptr->b);
    return 0;
}
