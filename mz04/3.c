#include <limits.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int
main(int argc, char **argv)
{
    long long n_min;
    long long off_min = 0;
    long long n;
    long long off = 0;

    int f;

    if ((f = open(argv[1], O_RDWR)) == -1) {
        return 0;
    }

    if (read(f, &n_min, sizeof(n_min)) != sizeof(n_min)) {
        close(f);
        return 0;
    }

    while (read(f, &n, sizeof(n)) == sizeof(n)) {
        off++;

        if (n < n_min) {
            n_min = n;
            off_min = off;
        }
    }

    if (n_min != LLONG_MIN) {
        n_min = -n_min;
        lseek(f, off_min * sizeof(n_min), SEEK_SET);

        if (write(f, &n_min, sizeof(n_min)) != sizeof(n_min)) {
            return 1;
        }
    }

    close(f);

    return 0;
}
