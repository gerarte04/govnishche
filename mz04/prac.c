#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

struct stat
{
    ino_t st_ino;
    dev_t st_dev;
    mode_t st_mode;
    nlink_t st_nlink;
    uid_t st_uid;
    gid_t st_gid;
    size_t st_size;
};

void f1(int argc, char **argv)
{
    struct stat s;
    stat(argv[1], &s);

    char m[9] = "rwxrwxrwx";

    for (int i = 0; i < 9 && s.st_mode; i++) {
        if (!(s.st_mode & 1)) {
            m[8 - i] = '-';
        }

        s.st_mode >>= 1;
    }

    printf("%s", m);
}

int my_access(const char* fname, int mode)
{
    struct stat buf;
    int user;

    if (!stat(fname, &buf) || !(user = get_uid())) {
        return 0;
    }

    mode_t a = buf.st_mode;
    int shift = 0;

    if (user == buf.st_uid) {
        shift = 6;
    } else if (get_gid() == buf.st_gid) {
        shift = 3;
    }

    return ((a >> shift) & mode == mode);
}

int main(int argc, char **argv)
{
    

    return 0;
}