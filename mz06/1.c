#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int
check_ext(char *buf)
{
    int len = strlen(buf);

    return len >= 4 && strcmp(buf + len - 4, ".exe") == 0;
}

int
check_cont(int buf_len, char *str1, char *str2)
{
    return buf_len > strlen(str1) + strlen(str2) + 1;
}

int
main(int argc, char **argv)
{
    DIR *dir = opendir(argv[1]);
    struct dirent *dd;

    char buf[PATH_MAX];
    int buf_len = sizeof(buf) / sizeof(buf[0]);

    int cnt = 0;

    while ((dd = readdir(dir)) != NULL) {
        snprintf(buf, buf_len, "%s/%s", argv[1], dd->d_name);

        struct stat s;

        if (check_cont(buf_len, argv[1], dd->d_name)) {
            if (stat(buf, &s) == 0 && S_ISREG(s.st_mode) && access(buf, X_OK) == 0 && check_ext(&buf[0])) {
                cnt++;
            }
        }
    }

    closedir(dir);

    printf("%d\n", cnt);

    return 0;
}
