#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>

enum
{
    MASK_4BIT = 0x0000000F,
    OFF1 = 20,
    OFF2 = 12,
    OFF3 = 8,
    BUF_SZ = 4
};

int
main(int argc, char **argv)
{
    unsigned int n;
    unsigned char buf[BUF_SZ];
    
    int f = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);

    while (scanf("%u", &n) == 1) {
        buf[0] = (unsigned char) (n >> OFF1 & MASK_4BIT);
        buf[1] = (unsigned char) (n >> OFF2);
        buf[2] = (unsigned char) (n >> OFF3 & MASK_4BIT);
        buf[3] = (unsigned char) n;
        write(f, buf, sizeof(buf[0]) * BUF_SZ);
    }

    close(f);

    return 0;
}
