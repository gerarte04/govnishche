#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
    DIR *d1;

    if ((d1 = opendir(argv[1])) == NULL) {
        return 1;
    }

    long long sum = 0;
    struct dirent *dd1;

    while ((dd1 = readdir(d1)) != NULL) {
        char buf[PATH_MAX];
        snprintf(buf, PATH_MAX, "%s/%s", argv[1], dd1->d_name);

        struct stat s1;

        if (lstat(buf, &s1) == 0 && S_ISREG(s1.st_mode) && access(buf, W_OK) == 0) {
            DIR *d2;

            if ((d2 = opendir(argv[2])) == NULL) {
                closedir(d1);
                return 1;
            }

            struct dirent *dd2;

            while ((dd2 = readdir(d2)) != NULL) {
                snprintf(buf, PATH_MAX, "%s/%s", argv[2], dd2->d_name);

                struct stat s2;

                if (stat(buf, &s2) == 0 && s1.st_ino == s2.st_ino && s1.st_dev == s2.st_dev) {
                    sum += s1.st_size;
                    break;
                }
            }

            closedir(d2);
        }
    }

    closedir(d1);

    printf("%lld\n", sum);

    return 0;
}
