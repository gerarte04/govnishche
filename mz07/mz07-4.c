#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
    struct stat s;
    lstat(argv[1], &s);

    int fd = open(argv[1], O_RDWR);
    int n;

    while (scanf("%d", &n) == 1) {
        unsigned char byte;
        int set = n > 0 && n <= s.st_size * CHAR_BIT;
        int unset = n < 0 && -n <= s.st_size * CHAR_BIT;

        if (!set && !unset) {
            continue;
        }

        if (unset) {
            n = -n;
        }

        unsigned int offs = (n - 1) / CHAR_BIT;
        unsigned int pos = (n % CHAR_BIT > 0) ? n % CHAR_BIT - 1 : CHAR_BIT - 1;

        lseek(fd, offs, SEEK_SET);
        read(fd, &byte, sizeof(byte));
        lseek(fd, -1, SEEK_CUR);

        if (set) {
            byte |= (unsigned char) (1u << pos);
        } else if (unset) {
            byte &= ~(unsigned char) (1u << pos);
        }

        write(fd, &byte, sizeof(byte));
    }

    close(fd);

    return 0;
}
