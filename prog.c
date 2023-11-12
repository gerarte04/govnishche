#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int
check_ext(char *buf)
{
    const char fmt[] = ".c";
    int len = strlen(buf);
    int ext_len = (sizeof(fmt) - 1) / sizeof(fmt[0]);

    return len >= ext_len && strcmp(buf + len - ext_len, fmt) == 0;
}

int
check_excl(char *buf)
{
    static const char *excl_list[4] = { "mz10", "mz11", "mz12", "prac.c" };
    static const int excl_size = 4;
    
    for (int i = 0; i < excl_size; i++) {
        if (strcmp(buf, excl_list[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

int
main(int argc, char **argv)
{
    DIR *d = opendir(".");
    struct dirent *dd;

    while (dd = readdir(d)) {
        if (check_excl(dd->d_name)) {
            continue;
        }

        char dir[PATH_MAX];
        getcwd(dir, PATH_MAX);

        char buf[PATH_MAX];
        snprintf(buf, sizeof(buf) / sizeof(buf[0]), "%s/%s", dir, dd->d_name);

        struct stat s;
        lstat(buf, &s);

        if (S_ISDIR(s.st_mode)) {
            DIR *d2 = opendir(buf);
            struct dirent *dd2;

            while (dd2 = readdir(d2)) {
                char buf2[PATH_MAX];
                snprintf(buf2, sizeof(buf2) / sizeof(buf2[0]), "%s/%s", buf, dd2->d_name);
                
                lstat(buf2, &s);

                if (S_ISREG(s.st_mode) && check_ext(dd2->d_name) && !check_excl(dd2->d_name)) {
                    char buf3[PATH_MAX];
                    snprintf(buf3, sizeof(buf3) / sizeof(buf3[0]), "%s/%s", buf, dd->d_name, dd2->d_name);

                    char buf4[PATH_MAX];
                    printf("%s\n", buf3);
                    snprintf(buf4, sizeof(buf4) / sizeof(buf4[0]), "git add \"%s\"", buf3);
                    system(buf4);
                }
            }

            closedir(d2);
        }
    }

    closedir(d);

    return 0;
}