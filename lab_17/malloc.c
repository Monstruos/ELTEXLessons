#include "malloc.h"


void *malloc(size_t size)
{
    struct chunk_header *ch_h = sbrk(sizeof(struct chunk_header));
    ch_h->is_allocated = true;
    ch_h->size = size;
    return sbrk(size);
}


void free(void *ptr)
{
    if (ptr != NULL) {
        struct chunk_header *ch_h = ptr - sizeof(struct chunk_header);
        ch_h->is_allocated = false;
    }
}
