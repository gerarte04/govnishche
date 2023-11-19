#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <linux/limits.h>
#include <dirent.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// решение на 270/300 баллов

int
check_ext(char *buf)
{
    int len = strlen(buf);

    return len >= 5 && strcmp(buf + len - 5, ".list") == 0;
}

int
main(int argc, char **argv)
{
    DIR *d = opendir(argv[1]);

    if (d == NULL) {
        return -1;
    }

    struct dirent *dd;
    unsigned long long sum = 0;

    while ((dd = readdir(d)) != NULL) {
        char buf[PATH_MAX];
        snprintf(buf, PATH_MAX, "%s/%s", argv[1], dd->d_name);

        struct stat s;

        if (lstat(buf, &s) == 0 && S_ISREG(s.st_mode) && access(buf, R_OK) == 0 && check_ext(dd->d_name)) {
            FILE *f = fopen(buf, "r");
            char buf2[PATH_MAX];

            struct stat s2;

            while (fgets(buf2, PATH_MAX, f)) {
                int len = strlen(buf2);

                if (buf2[len - 1] == '\n') {
                    buf2[len - 1] = '\0';
                }

                if (buf2[len - 2] == '\r') {
                    buf2[len - 2] = '\0';
                }

                if (lstat(buf2, &s2) == 0 && S_ISREG(s2.st_mode) && access(buf2, X_OK) == 0) {
                    sum += s2.st_size;
                }
            }

            fclose(f);
        }
    }

    closedir(d);

    printf("%llu\n", sum);

    return 0;
}
