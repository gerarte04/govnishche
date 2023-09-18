#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
    long pos = 0;
    long neg = 0;

    for (int i = 1; i < argc; i++) {
        char *buf = NULL;
        errno = 0;

        long a = strtol(argv[i], &buf, 10);

        if (*buf || errno || buf == argv[i] || (int) a != a) {
            continue;
        }

        if (a > 0) {
            pos += a;
        } else {
            neg += a;
        }
    }

    printf("%ld\n%ld\n", pos, neg);

    return 0;
}
