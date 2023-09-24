#include <errno.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
    char *buf = NULL;
    int f = open(argv[1], O_RDWR);
    int n = (int) strtol(argv[2], &buf, 10);

    double a, b;
    if (read(f, &a, sizeof(a)) <= 0) {
        close(f);
        return 0;
    }

    for (int i = 1; i < n && read(f, &b, sizeof(b)) > 0; i++) {
        lseek(f, -sizeof(b), SEEK_CUR);
        a = b - a;
        write(f, &a, sizeof(a));
    }

    close(f);

    return 0;
}
