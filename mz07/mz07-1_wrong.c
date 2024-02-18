#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define ERR "18446744073709551616"

// не проходит сука почему-то

enum
{
    BASE = 3
};

int
main(void)
{
    long long num = 0;
    int printable = 0;
    int err = 0;

    int c;

    while ((c = getc(stdin)) != EOF) {
        if (c == '\n' || c == ' ') {
            if (printable && !err) {
                printf("%lld\n", num);
            }

            num = 0;
            printable = 0;
            err = 0;
        } else if (!err) {
            int n = 0;

            switch (c) {
            case '1':
                n = 1;
                break;
            case '0':
                n = 0;
                break;
            case 'a':
                n = -1;
                break;
            default:
                err = 1;
            }

            if (!err && __builtin_mul_overflow(num, BASE, &num)) {
                if (__builtin_add_overflow(num, 1, &num) && n == '1') {
                    printf("%lld\n", LLONG_MIN);
                } else {
                    printf("%s\n", ERR);
                    printable = 0;
                }

                err = 1;
            }

            if (!err && __builtin_add_overflow(num, n, &num)) {
                printf("%s\n", ERR);
                printable = 0;
                err = 1;
            }

            if (!err) {
                printable = 1;
            }
        }
    }

    return 0;
}
