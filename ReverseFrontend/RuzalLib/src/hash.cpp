#include "../include/hash.h"
#include <string.h>

unsigned long long HashStack(Stack* stk) {
    char* struct_ptr = (char*)stk;
    return GavGavHash((char*)stk->data - sizeof(Canary_t), stk->capacity * sizeof(Elem_t) + 2 * sizeof(Canary_t))
        + GavGavHash(struct_ptr, sizeof(*stk));
}

unsigned long long GavGavHash(char* data, size_t size)
{
    unsigned int m = 0x5bd1e995;
    unsigned int seed = 0;
    unsigned int r = 24;
    unsigned int h = seed ^ size;

    unsigned char* arr = (unsigned char*)data;
    unsigned int k = 0;

    while (size >= 4)
    {
        k = arr[0] | (arr[1] << 8) | (arr[2] << 16) | (arr[3] << 24);

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        arr += 4;
        size -= 4;
    }

    switch (size)
    {
    case 3:
        h ^= arr[2] << 16;
    case 2:
        h ^= arr[1] << 8;
    case 1:
        h ^= arr[0];
        h *= m;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}