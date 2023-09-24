#include <errno.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/stat.h>

int
main(int argc, char **argv)
{
    char *buf = NULL;
    int f = open(argv[1], O_RDWR, 0777);
    int n = (int) strtol(argv[2], &buf, 10);

    double a, b;
    read(f, &a, sizeof(a));

    for (int i = 1; i < n; i++) {
        read(f, &b, sizeof(b));
        lseek(f, -sizeof(b), 1);
        a = b - a;
        write(f, &a, sizeof(a));
    }

    close(f);

    return 0;
}