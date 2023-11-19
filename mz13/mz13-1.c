#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

enum
{
    PRC_CNT = 4,
    YR_OFF = 1900
};

int
main(int argc, char **argv)
{
    int fd[2];
    pipe(fd);

    for (int i = 0; i < PRC_CNT; i++) {
        if (i == PRC_CNT - 1) {
            close(fd[0]);

            time_t t = time(NULL);

            for (int i = 1; i < PRC_CNT; i++) {
                write(fd[1], &t, sizeof(t));
            }

            close(fd[1]);
            exit(0);
        }

        if (fork()) {
            close(fd[1]);
            wait(NULL);

            time_t t;
            read(fd[0], &t, sizeof(t));
            close(fd[0]);

            struct tm tt;
            memset(&tt, 0, sizeof(tt));
            localtime_r(&t, &tt);

            switch (i) {
            case 0:
                printf("Y:%04d\n", tt.tm_year + YR_OFF);
                break;
            case 1:
                printf("M:%02d\n", tt.tm_mon + 1);
                break;
            case 2:
                printf("D:%02d\n", tt.tm_mday);
                break;
            }

            fflush(stdout);

            exit(0);
        }
    }

    return 0;
}
