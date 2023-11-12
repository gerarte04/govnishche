#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
    long long res = 0;

    for (int i = 1; i < argc; i++) {
        char *buf = NULL;
        errno = 0;
        long n = strtol(argv[i], &buf, 10);

        if (errno || buf == argv[i] || (int) n != n) {
            continue;
        }

        if (buf[0] == 'k') {
            if (__builtin_mul_overflow(n, 1000, &n)) {
                continue;
            }

            buf++;
        }

        if (buf[0] == '+' && buf[1] == '\0') {
            res += n;
        } else if (buf[0] == '-' && buf[1] == '\0') {
            res -= n;
        }
    }

    printf("%lld\n", res);

    return 0;
}