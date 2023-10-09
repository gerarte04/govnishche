#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void f1(int argc, char **argv)
{
    DIR *d = opendir(argv[1]);
    struct dirent *dd;

    while (dd = readdir(d)) {
        char path[PATH_MAX + 1];
        snprintf(path, PATH_MAX, "%s/%s", argv[1], dd->d_name);

        struct stat s;
        stat(path, &s);

        if (S_ISREG(s.st_mode)) {
            printf("%s\n", dd->d_name);
        }
    }

    closedir(d);
}

void f2(int argc, char **argv)
{
    DIR *d = opendir(argv[1]);
    struct dirent *dd;

    while ((dd = readdir(d)) != NULL) {
        char buf[PATH_MAX + 1];
        snprintf(buf, PATH_MAX, "%s/%s", argv[1], dd->d_name);

        if (access(buf, F_OK | R_OK | W_OK) == -1) {
            continue;
        }

        char *name = dd->d_name;
        char *last = name;

        while (*name != '\0') {
            if (*name == '.') {
                last = name;
            }

            name++;
        }

        if (strcmp(last, ".bak") == 0) {
            if (unlink(buf) == -1) {
                return;
            }
        }
    }

    closedir(d);
}

void f3(int argc, char **argv)
{
    DIR *dir = opendir(argv[1]);
    struct dirent *dd;

    while (dd = readdir(dir)) {
        char buf[PATH_MAX + 1];
        snprintf(buf, PATH_MAX, "%s/%s", argv[1], dd->d_name);

        DIR *inner_dir;

        if (inner_dir = opendir(buf)) {
            struct dirent *inner_dd;
            int flag = 1;

            while (inner_dd = readdir(inner_dir)) {
                char inner_buf[2 * PATH_MAX + 1];
                snprintf(inner_buf, 2 * PATH_MAX, "%s/%s", buf, inner_dd->d_name);
                struct stat s;
                if (stat(inner_buf, &s) == -1) {
                    continue;
                }

                if (!S_ISDIR(s.st_mode) || ((strcmp(inner_dd->d_name, ".") != 0 && strcmp(inner_dd->d_name, "..") != 0))) {
                    rmdir(inner_dd->d_name);
                    break;
                }
            }
        }
    }
}

int main(int argc, char **argv)
{
    struct stat st_cur;
    stat(".", &st_cur);

    struct stat st_sl;
    stat("/", &st_sl);

    if (st_sl.st_ino == st_cur.st_ino && st_sl.st_dev == st_cur.st_dev) {
        printf("we're in root\n");
        return 0;
    }

    chdir("..");

    DIR *d = opendir(".");
    struct dirent *dd;

    while (dd = readdir(d)) {
        char buf[PATH_MAX + 1];
        snprintf(buf, PATH_MAX, "./%s", dd->d_name);

        struct stat s;

        if (stat(buf, &s) == 0 && s.st_ino == st_cur.st_ino && s.st_dev == st_cur.st_dev) {
            printf("%s\n", dd->d_name);
            break;
        }
    }

    closedir(d);

    return 0;
}