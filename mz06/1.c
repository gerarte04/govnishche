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

    int cnt = 0;

    while ((dd = readdir(dir)) != NULL) {
        char buf[PATH_MAX + 1];
        snprintf(buf, PATH_MAX, "%s/%s", argv[1], dd->d_name);

        struct stat s;
        int len;

        int check_ext = ((len = strlen(buf)) >= 4 && strcmp(buf + len - 4, ".exe") == 0);

        if (stat(buf, &s) == 0 && S_ISREG(s.st_mode) && access(buf, X_OK) == 0 && check_ext) {
            cnt++;
        }
    }

    printf("%d\n", cnt);

    closedir(dir);

    return 0;
}
