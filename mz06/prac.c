#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

enum
{
    TWO_WEEKS = 60 * 60 * 24 * 14
};

void f1(int *y, int *m, int *d, int offset)
{
    struct tm tt;
    memset(&tt, 0, sizeof(tt));
    tt.tm_year = *y - 1900;
    tt.tm_mon = *m - 1;
    tt.tm_mday = *d;
    tt.tm_mday += offset;
    tt.tm_isdst = -1;

    mktime(&tt);

    *y = tt.tm_year + 1900;
    *m = tt.tm_mon + 1;
    *d = tt.tm_mday;
}

void f2(int argc, char **argv)
{
    DIR *d = opendir(argv[1]);
    struct dirent *dd;
    const time_t t = time(NULL);

    while (dd = readdir(d)) {
        char path[PATH_MAX + 1];
        snprintf(path, PATH_MAX, "%s/%s", argv[1], dd->d_name);

        struct stat s;
        lstat(path, &s);
        
        if(S_ISREG(s.st_mode) && t - s.st_mtime <= TWO_WEEKS)
        {
            char new_path[PATH_MAX + 1];
            snprintf(new_path, PATH_MAX, "%s/%s", argv[2], dd->d_name);
            rename(path, new_path);
        }
    }

    closedir(d);
}

void f3(int argc, char **argv)
{
    char *buf = NULL;
    struct tm tt;

    int y = (int) strtol(argv[1], &buf, 10) - 1900;
    int d = (int) strtol(argv[2], &buf, 10);
    int wd = (int) strtol(argv[3], &buf, 10);

    int cnt = 0;

    for (int i = 0; i < 12; i++) {
        memset(&tt, 0, sizeof(tt));
        tt.tm_year = y;
        tt.tm_mon = i;
        tt.tm_mday = d;
        tt.tm_isdst = -1;

        mktime(&tt);

        if (tt.tm_wday == wd && tt.tm_mon == i) {
            cnt++;
        }
    }

    printf("%d\n", cnt);
}

void f4(int argc, char **argv)
{
    FILE *f = fopen(argv[1], O_RDONLY);
    int y, m, d;
    int hr, min, sec;
    struct tm tt;
    struct tm new_tt;
    memset(&tt, 0, sizeof(tt));
    fscanf(f, "%d/%d/%d", &tt.tm_year, &tt.tm_mon, &tt.tm_mday);
    fscanf(f, "%d:%d:%d", &tt.tm_hour, &tt.tm_min, &tt.tm_sec);

    while (fscanf(f, "%d/%d/%d", &tt.tm_year, &tt.tm_mon, &tt.tm_mday) == 1
        && fscanf(f, "%d:%d:%d", &tt.tm_hour, &tt.tm_min, &tt.tm_sec) == 1) {
        
    }

    fclose(f);
}

int main(int argc, char **argv)
{
    f4(argc, argv);

    return 0;
}