#include <dirent.h>
#include <errno.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

enum
{
    MAX_DEPTH = 4
};

void print_files(char *abs, char *rel, off_t max_size, int depth)
{
    DIR *dir;

    if (!(dir = opendir(abs))) {
        return;
    }

    struct dirent *dd;
    char *fmt_str = (rel[0] == '\0') ? "%s%s" : "%s/%s";

    while (dd = readdir(dir)) {
        char buf_dd[PATH_MAX + 1];
        snprintf(buf_dd, PATH_MAX, "%s/%s", abs, dd->d_name);

        struct stat s;

        if (lstat(buf_dd, &s) == 0) {
            if (S_ISREG(s.st_mode) && access(buf_dd, R_OK) == 0 && s.st_size <= max_size) {
                printf(fmt_str, rel, dd->d_name);
                putc('\n', stdout);
            } else if (S_ISDIR(s.st_mode) && depth < MAX_DEPTH
                && strcmp(dd->d_name, ".") != 0 && strcmp(dd->d_name, "..") != 0) {
                char new_rel[PATH_MAX + 1];
                snprintf(new_rel, PATH_MAX, fmt_str, rel, dd->d_name);

                print_files(buf_dd, new_rel, max_size, depth + 1);
            }
        }
    }

    closedir(dir);
}

int
main(int argc, char **argv)
{
    char *buf = NULL;
    errno = 0;
    off_t max_size = (off_t) strtoul(argv[2], &buf, 10);

    if (errno || *buf || argv[1] == buf) {
        return 1;
    }

    print_files(argv[1], "", max_size, 1);

    return 0;
}
