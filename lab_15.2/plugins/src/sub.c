#define F_NAME_LENGTH 256
#define F_COUNT 1
const int f_count = F_COUNT;
const char f_name[F_COUNT][F_NAME_LENGTH] = {"sub"};


float sub(int first, int second)
{
    return (float) (first - second);
}
