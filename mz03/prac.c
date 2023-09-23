#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>

// int open(const char *fname, int flags, int mode)
// int fd = open("aaa", O_CREAT | O_TRUNC | O_RDONLY);
// O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC, O_APPEND, O_EXCL, O_RDWR
// int fd = open("aaa", 0777);

// close(fd);

// ssize_t read(int fd, void *buf, size_t nbytes)
// ssize_t write(int fd, void *buf, size_t nbytes)

void
f1 (int argc, char *argv[]) {
    int fr = open(argv[1], O_CREAT | O_RDONLY, 0777);
    int fw = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0777);

    char buf[1024];
    int c;
    while ((c = read(fr,  buf, 1024)) > 0) {
        char *p = buf;
        int n;
        while (c) {
            n = write(fw, p, c);
            c -= n;
            p += n;
        }
    }

    close(fr);
    close(fw);

    return 0;
}


// off_t lseek(int fd, off_t offset, int mode);
// SEEK_SET 0, SEEK_CUR 1, SEEK_END 2

void
f2(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDWR, 0777);
    long a;
    long b;

    int n = lseek(fd, -8, 2) / 8;
    lseek(fd, 0, 0);

    for (int i = 0; i < n / 2; i++){
        read(fd, &a, 8);
        lseek(fd, -8 * (i + 1), 2);
        read(fd, &b, 8);
        lseek(fd, 8, 1);
        write(fd, &a, 8);
        lseek(fd, 8 * i , 0);
        write(fd, &b, 8);
    }
    close(fd);

    return 0;
}

int
main (int argc, char *argv[]) {
    int fd = open(argv[1], O_RDWR, 0777);
    unsigned char buf[4];
    int c = 0, s = 0;
    while ((c = read(fd,  buf, sizeof(buf))) > 0) {
        for (int j = 0; j < 4; j++) {
            s += buf[j] << (24 - j * 8);
        }
    }    

    printf("%d\n", s);
    
    close(fd);

    return 0;
}
