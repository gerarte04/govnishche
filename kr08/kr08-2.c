#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// решение на 60/300 баллов, вероятно, нужно в 22 строке заменить arr[i] на arr[i - 1] (я долбоеб убейте меня)

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
            new_n |= ((n >> arr[i] - 1) & 1) << i;
        }

        printf("%hhu\n", new_n);
    }

    return 0;
}
