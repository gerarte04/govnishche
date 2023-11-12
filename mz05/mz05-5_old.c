#include <limits.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

enum
{
    BUF_SIZE = 4096,
    MASK = 0777
};

int
copy_file(const char *srcpath, const char *dstpath)
{
    struct stat st_src;
    struct stat st_dst;

    if (access(srcpath, F_OK | R_OK) != 0 || stat(srcpath, &st_src) != 0) {
        return -1;
    }

    int fsrc;
    int fdst;

    int stat_code;

    if ((stat_code = stat(dstpath, &st_dst)) == 0 && S_ISDIR(st_dst.st_mode)) {
        const char *str = srcpath;
        const char *last = str;

        while (*str != '\0') {
            if (*str == '/') {
                last = str + 1;
            }

            str++;
        }

        char new_dest[PATH_MAX + 1];
        snprintf(new_dest, PATH_MAX + 1, "%s/%s", dstpath, last);

        struct stat st_new_dest;

        if (stat(new_dest, &st_new_dest) == 0) {
            if (st_src.st_ino == st_new_dest.st_ino && st_src.st_dev == st_new_dest.st_dev) {
                return 0;
            }
        }

        if ((fdst = open(new_dest, O_WRONLY | O_CREAT | O_TRUNC, st_src.st_mode & MASK)) == -1) {
            return -1;
        }
    } else {
        if (stat_code == 0 && st_src.st_ino == st_dst.st_ino && st_src.st_dev == st_dst.st_dev) {
            return 0;
        }

        if ((fdst = open(dstpath, O_WRONLY | O_CREAT | O_TRUNC, st_src.st_mode & MASK)) == -1) {
            return -1;
        }
    }

    if ((fsrc = open(srcpath, O_RDONLY)) == -1) {
        return -1;
    }

    unsigned char buf[BUF_SIZE];
    int cnt_read;

    while ((cnt_read = read(fsrc, buf, sizeof(buf[0]) * BUF_SIZE)) > 0) {
        if (write(fdst, buf, sizeof(buf[0]) * cnt_read) == -1) {
            return -1;
        }
    }

    close(fsrc);
    close(fdst);

    return 0;
}

int main(void)
{
    if (copy_file("5.c", "/home/kingmidas/Desktop/fuck.c") == 0) {
        printf("success\n");
    } else {
        printf("not success\n");
    }

    return 0;
}
