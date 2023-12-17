#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <unistd.h>

enum
{
    BSIZE = 512
};

int main(int argc, char **argv)
{
    unsigned cur, off;
    scanf("%u %u", &cur, &off);
    int fd = open(argv[1], O_RDONLY);

    while (off >= BSIZE) {
        unsigned char data[2];
        lseek(fd, cur * 3 / 2, SEEK_SET);
        read(fd, data, sizeof(data));

        if (cur % 2 == 0) {
            cur = data[0] | (data[1] & 0xF) << 8;
        } else {
            cur = data[0] >> 4 | (unsigned) data[1] << 4;
        }
        
        off -= BSIZE;
    }

    printf("%u %u\n", cur, off);
    fflush(stdout);
    close(fd);

    return 0;
}
