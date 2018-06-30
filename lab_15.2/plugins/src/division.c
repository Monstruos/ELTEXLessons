#include <float.h>
#define F_NAME_LENGTH 256
#define F_COUNT 1
const int f_count = F_COUNT;
const char f_name[F_COUNT][F_NAME_LENGTH] = {"division"};


float division(int first, int second)
{
    return (float) (second == 0 ? FLT_MAX : ((float) first) / second);
}
