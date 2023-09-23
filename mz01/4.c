#include <stdlib.h>

int
comp(const void *x, const void *y)
{
    int a = *(int *) x;
    int b = *(int *) y;
    int ar = a & 1;
    int br = b & 1;

    if (ar != br) {
        return ar - br;
    } else if (a == b) {
        return 0;
    } else if (ar == 0) {
        if (a < b) {
            return -1;
        }

        return 1;
    } else if (b < a) {
        return -1;
    }

    return 1;
}

void
sort_even_odd(size_t count, int *data)
{
    qsort(data, count, sizeof(*data), comp);
}
