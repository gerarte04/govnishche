#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
    struct stat s;
    lstat(argv[1], &s);

    unsigned char buf[s.st_size];

    int fd = open(argv[1], O_RDWR);

    for (int i = 0; i < s.st_size; i++) {
        if (read(fd, &buf[i], sizeof(buf[i])) != sizeof(buf[i])) {
            return -1;
        }

        unsigned char new_n = 0;

        for (int j = 0; j < 8; j++) {
            new_n |= ((buf[i] >> j) & 1) << (7 - j);
        }

        buf[i] = new_n;
    }

    for (int i = 0; i < s.st_size / 2; i++) {
        unsigned char temp = buf[i];
        buf[i] = buf[s.st_size - i - 1];
        buf[s.st_size - i - 1] = temp;
    }

    lseek(fd, 0, SEEK_SET);

    for (int i = 0; i < s.st_size; i++) {
        if (write(fd, &buf[i], sizeof(buf[i])) != sizeof(buf[i])) {
            return -1;
        }
    }

    close(fd);

    return 0;
}
