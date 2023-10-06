#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
    DIR *dir = opendir(argv[1]);
    struct dirent *dd;

    long long cnt = 0;

    while (dd = readdir(dir)) {
        char buf[PATH_MAX + 1];
        snprintf(buf, PATH_MAX, "%s/%s", argv[1], dd->d_name);

        struct stat s;
        int len;

        if (stat(buf, &s) == 0 && S_ISREG(s.st_mode) && access(buf, X_OK) == 0
            && (len = strlen(buf)) >= 4 && strcmp(buf + len - 4, ".exe") == 0) {
            cnt++;
        }
    }

    printf("%lld", cnt);

    closedir(dir);

    return 0;
}
