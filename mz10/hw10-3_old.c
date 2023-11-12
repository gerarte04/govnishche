#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

enum
{
    BASE = 10
};

int
main(int argc, char **argv)
{
    char *buf = NULL;
    int rows = (int) strtol(argv[2], &buf, BASE);
    int cols = (int) strtol(argv[3], &buf, BASE);
    int size;

    int fd = open(argv[1], O_RDWR);

    if (fd < 0) {
        return -1;
    }

    int *mx = mmap(NULL, (size = rows * cols * sizeof(*mx)), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ftruncate(fd, size);
    close(fd);

    if (mx == MAP_FAILED) {
        return -1;
    }

    int w = cols;
    int h = rows - 1;
    int num = 0;
    int offx = -1;
    int offy = 0;
    int dir = 1;

    while (w > 0 && h >= 0) {
        for (int i = 0; i < w; i++) {
            offx += dir;
            mx[offx + offy] = ++num;
        }

        for (int j = 0; j < h; j++) {
            offy += dir * cols;
            mx[offx + offy] = ++num;
        }

        w--;
        h--;
        dir = -dir;
    }

    munmap(mx, size);

    return 0;
}