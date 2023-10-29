#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

enum
{
    MEM_SIZE = 64 * 1024,
    NUM_SIZE = 2,
    MASK_9BIT = 0x01FF,
    HIGH_OFF = 9,
    BASE = 16
};

int
main(int argc, char **argv)
{
    char *buf = NULL;
    unsigned short start_addr = (unsigned short) strtoul(argv[2], &buf, BASE);
    unsigned short virt_addr;

    int fd = open(argv[1], O_RDONLY);

    if (fd < 0) {
        return -1;
    }

    unsigned short *mem = mmap(NULL, MEM_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    close(fd);

    if (mem == MAP_FAILED) {
        return -1;
    }

    while (scanf("%hx", &virt_addr) == 1) {
        unsigned short virt_page = start_addr / NUM_SIZE + (virt_addr >> HIGH_OFF);
        unsigned short phys_addr = virt_addr & MASK_9BIT | mem[virt_page];
        printf("%hu\n", mem[phys_addr / NUM_SIZE]);
    }

    munmap(mem, MEM_SIZE);

    return 0;
}
