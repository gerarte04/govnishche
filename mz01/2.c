#include <stdio.h>
#include <stdlib.h>

enum
{
    MASK_2ND = 251,
    MASK_3RD = 8,
    OFFSET_NUM = '0' - 1,
    OFFSET_LOWER = 'a' - 10 - 1,
    OFFSET_UPPER = 'A' - 10 - 26 - 1
};

int
main(void)
{
    int c;

    while ((c = fgetc(stdin)) != EOF) {
        if (c >= '0' && c <= '9') {
            c -= OFFSET_NUM;
        } else if (c >= 'a' && c <= 'z') {
            c -= OFFSET_LOWER;
        } else if (c >= 'A' && c <= 'Z') {
            c -= OFFSET_UPPER;
        } else {
            continue;
        }

        c = ((c & MASK_2ND) ^ MASK_3RD);

        if (c == 0) {
            fputc('@', stdout);
        } else if (c == 63) {
            fputc('#', stdout);
        } else if (c >= 1 && c <= 10) {
            fputc(c + OFFSET_NUM, stdout);
        } else if (c >= 11 && c <= 36) {
            fputc(c + OFFSET_LOWER, stdout);
        } else if (c >= 37 && c <= 62) {
            fputc(c + OFFSET_UPPER, stdout);
        }
    }

    return 0;
}
