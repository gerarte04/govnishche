#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

enum
{
    KIB = 1024
};

int
main(int argc, char **argv)
{
    long long sum = 0;

    for (int i = 1; i < argc; i++) {
        struct stat buf;

        if (lstat(argv[i], &buf) != 0) {
            continue;
        }

        if (buf.st_size % KIB == 0 && S_ISREG(buf.st_mode) && buf.st_nlink == 1) {
            sum += buf.st_size;
        }
    }

    printf("%lld\n", sum);

    return 0;
}
