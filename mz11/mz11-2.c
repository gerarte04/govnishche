#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

enum
{
    PRC_CNT = 3
};

int
main(int argc, char **argv)
{
    for (int i = 1; i <= PRC_CNT; i++) {
        if (i < PRC_CNT) {
            pid_t pid = fork();

            if (pid < 0) {
                exit(1);
            }

            if (pid) {
                wait(NULL);

                char *fmt = (i == 1) ? "%d\n" : "%d ";
                printf(fmt, i);

                exit(0);
            }
        } else {
            printf("%d ", i);
        }
    }

    return 0;
}
