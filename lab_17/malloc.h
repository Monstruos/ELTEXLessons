#include <unistd.h>
#include <stdbool.h>

struct chunk_header {
    bool is_allocated;
    size_t size;
};

void *malloc(size_t size);
void free(void *ptr);
