#include <stdio.h>
#include "recur.h"

int main()
{
    int start_depth = 0;
    int depth_var = recur(start_depth);
    printf("%i\n", depth_var);
    return 0;
}
