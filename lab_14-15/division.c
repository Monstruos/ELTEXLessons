#include <float.h>

float division(int first, int second)
{
    return (float) (second == 0 ? FLT_MAX : ((float) first) / second);
}
