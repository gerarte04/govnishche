#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

ssize_t
getcwd2(int fd, char *buf, size_t size)
{
    DIR *start = opendir(".");

    if (fchdir(fd) == -1) {
        goto err;
    }

    struct stat s_fd;
    struct stat s_sl;

    if (lstat("/", &s_sl) != 0) {
        goto err;
    }

    if (lstat(".", &s_fd) != 0) {
        goto err;
    }

    char *path = calloc(PATH_MAX + 1, sizeof(path[0]));

    while (!(s_fd.st_ino == s_sl.st_ino && s_fd.st_dev == s_sl.st_dev)) {
        chdir("..");

        DIR *dir;
        struct dirent *dd;

        if ((dir = opendir(".")) == NULL) {
            free(path);
            goto err;
        }

        int found = 0;

        while ((dd = readdir(dir)) != NULL) {
            char buf_abs[PATH_MAX + 1];
            snprintf(buf_abs, PATH_MAX, "./%s", dd->d_name);

            struct stat s;

            if (lstat(buf_abs, &s) == 0 && s.st_ino == s_fd.st_ino && s.st_dev == s_fd.st_dev) {
                found = 1;

                char new_path[PATH_MAX + 1];
                snprintf(new_path, PATH_MAX, "/%s%s", dd->d_name, path);
                free(path);
                path = strdup(new_path);

                break;
            }
        }

        closedir(dir);

        if (!found || lstat(".", &s_fd) != 0) {
            free(path);
            goto err;
        }
    }

    fchdir(dirfd(start));
    closedir(start);

    if (*path == '\0') {
        *path = '/';
        *(path + 1) = '\0';
    }

    snprintf(buf, size, "%s", path);
    int len = strlen(path);
    free(path);

    return len;

err:
    fchdir(dirfd(start));
    closedir(start);
    return -1;
}
