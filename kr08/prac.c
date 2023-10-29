#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

// valgrind --leak-check=full ./test - поиск утечек динамической памяти

void f1(int argc, char **argv)
{
    int fd1 = open(argv[1], O_RDONLY);
    int fd2 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0777);

    struct stat s;
    stat(argv[1], &s);

    char *addr1 = mmap(NULL, s.st_size, PROT_READ, MAP_SHARED, fd1, 0);
    close(fd1);

    ftruncate(fd2, s.st_size);
    char *addr2 = mmap(NULL, s.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
    close(fd2);

    for (int i = 0; i < s.st_size; i++) {
        addr2[s.st_size - i - 1] = addr1[i];
    }

    munmap(addr1, s.st_size);
    munmap(addr2, s.st_size);
}

void f2(int argc, char **argv)
{
    int fd = open(argv[1], O_RDWR);

    struct stat s;
    stat(argv[1], &s);

    ftruncate(fd, s.st_size + 5);

    char *addr1 = mmap(NULL, s.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    memmove(addr1 + 6, addr1, s.st_size);
    memset(addr1, ' ', 5);

    munmap(addr1, s.st_size);
}

int main(int argc, char **argv)
{
    

    return 0;
}