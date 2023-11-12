#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char **argv)
{
    unsigned char n;

    int arr[8];

    for (int i = 0; i < 8; i++) {
        sscanf(argv[i + 1], "%d", &arr[i]);
    }

    while (scanf("%hhu", &n) == 1) {
        unsigned char new_n = 0;

        for (int i = 0; i < 8; i++) {
            new_n |= ((n >> arr[i]) & 1) << i;
        }

        printf("%hhu\n", new_n);
    }

    return 0;
}
