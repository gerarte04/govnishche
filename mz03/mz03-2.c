#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
    long long pos = 0;
    long long neg = 0;

    for (int i = 1; i < argc; i++) {
        char *buf = NULL;
        errno = 0;

        long read = strtol(argv[i], &buf, 10);
        int arg = (int) read;

        if (*buf || errno || buf == argv[i] || arg != read) {
            continue;
        }

        if (arg > 0) {
            pos += arg;
        } else {
            neg += arg;
        }
    }

    printf("%lld\n%lld\n", pos, neg);

    return 0;
}
