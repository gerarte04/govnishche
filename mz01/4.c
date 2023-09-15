#include <stdlib.h>

int comp(const void *x, const void *y) {
    int a = *(int*) x;
    int b = *(int*) y;
    int ar = abs(a % 2);
    int br = abs(b % 2);

    return (ar == br) ? (1 - ar - br) * ((a > b) - (a < b)) : ar - br;
}

void sort_even_odd(size_t count, int *data) {
    qsort(data, count, sizeof(int), comp);
}
