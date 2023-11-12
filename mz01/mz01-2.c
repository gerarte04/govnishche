#include <stdio.h>
#include <stdlib.h>

enum
{
    MASK_2ND = 251,
    MASK_3RD = 8,
    AT_CODE = 0,
    SHARP_CODE = 63,
    NUM_START = 1,
    LOWER_START = NUM_START + ('9' - '0' + 1),
    UPPER_START = LOWER_START + ('z' - 'a' + 1),
    NUM_END = LOWER_START - 1,
    LOWER_END = UPPER_START - 1,
    UPPER_END = UPPER_START + ('Z' - 'A'),
    OFFSET_NUM = '0' - NUM_START,
    OFFSET_LOWER = 'a' - LOWER_START,
    OFFSET_UPPER = 'A' - UPPER_START
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

        if (c == AT_CODE) {
            fputc('@', stdout);
        } else if (c == SHARP_CODE) {
            fputc('#', stdout);
        } else if (c >= NUM_START && c <= NUM_END) {
            fputc(c + OFFSET_NUM, stdout);
        } else if (c >= LOWER_START && c <= LOWER_END) {
            fputc(c + OFFSET_LOWER, stdout);
        } else if (c >= UPPER_START && c <= UPPER_END) {
            fputc(c + OFFSET_UPPER, stdout);
        }
    }

    return 0;
}
