#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>

struct myfile
{
    int fd;
    int size;
    char *buf;
    int pos;
    int n;
};

void f1(void)
{
    char s[100];

    while (fgets(s, 100, stdin)) {
        double sum = 0;
        double a;
        int n;
        int off = 0;

        while (sscanf(&s[off], "%lf%n", &a, &n)) {
            sum += a;
            off += n;
        }

        printf("%lf\n", sum);
    }
}

void f2(int argc, char **argv)
{
    FILE *f = fopen(argv[1], O_RDONLY);
    char s[PATH_MAX];

    while (fgets(s, PATH_MAX, f)) {
        char buf[PATH_MAX];
        s[strlen(s) - 1] = '\0';
        snprintf(buf, PATH_MAX, "%s/%s", argv[2], s);

        struct stat st;

        if (lstat(buf, &st) < 0 || !S_ISREG(st.st_mode)) {
            printf("%s\n", s);
        }
    }

    fclose(f);
}

void f3(int argc, char **argv)
{
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);

    unsigned short n;

    while (scanf("%u", n) == 1) {
        unsigned char c[2];
        c[0] = n >> 8;
        c[1] = n;
        write(fd, c, sizeof(c));
    }

    close(fd);
}

int mygetc(struct myfile *f)
{
    if (f->pos == f->n) {
        f->pos = 0;
        f->n = read(f->fd, f->buf, f->size);

        if (f->n == 0) {
            return EOF;
        }
    }

    return (unsigned char) f->buf[f->pos++];
}

int main(int argc, char **argv)
{
    

    return 0;
}