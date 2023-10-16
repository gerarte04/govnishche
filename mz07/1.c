#include <stdio.h>
#include <stdlib.h>

#define ERR "18446744073709551616"

enum
{
    START_CAP = 16,
    BASE = 3
};

char *
read_str(int *len)
{
    int c;

    while ((c = getc(stdin)) == ' ' || c == '\n');

    if (c == EOF) {
        *len = 0;
        return NULL;
    }

    *len = 1;
    int cap = START_CAP;
    char *str = malloc(cap * sizeof(str[0]));
    str[0] = c;

    while ((c = getc(stdin)) != ' ' && c != '\n') {
        if (*len >= cap) {
            cap *= 2;
            str = realloc(str, cap * sizeof(str[0]));
        }

        str[*len] = c;
        (*len)++;
    }

    return str;
}

int
main(void)
{
    char *str;
    int len;

    while ((str = read_str(&len)) != NULL) {
        long long num = 0;
        int n;
        int err = 0;

        for (int i = 0; i < len; i++) {
            switch (str[i]) {
            case '1':
                n = 1;
                break;
            case '0':
                n = 0;
                break;
            case 'a':
                n = -1;
                break;
            }

            if (__builtin_mul_overflow(num, BASE, &num)) {
                if (__builtin_add_overflow(num, 1, &num)) {
                    n--;
                } else {
                    printf("%s\n", ERR);
                    err = 1;
                    break;
                }
            }

            if (__builtin_add_overflow(num, n, &num)) {
                printf("%s\n", ERR);
                err = 1;
                break;
            }
        }

        if (!err) {
            printf("%lld\n", num);
        }

        free(str);
    }

    return 0;
}