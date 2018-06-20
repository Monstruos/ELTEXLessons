#define MAX_DEPTH 5

int recur(int depth)
{
    int dead_var;
    if (depth < MAX_DEPTH) {
        int new_depth = depth + 1;
        return recur(new_depth);
    } else {
        return depth;
    }
}
