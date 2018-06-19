#ifndef PACK_NONPACK_STRUCTS
#define PACK_NONPACK_STRUCTS

struct nonpack {
    char a;
    int b;
};

struct pack {
    char a;
    int b;
} __attribute__((packed));

#endif // PACK_NONPACK_STRUCTS
