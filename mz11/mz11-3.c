#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <unistd.h>

enum
{
    PRC_CNT = 3,
    STR_LEN = 8,
    BASE = 10
};

int
main(int argc, char **argv)
{
    for (int i = 1; i <= PRC_CNT; i++) {
        pid_t pid = fork();

        if (!pid) {
            char s[STR_LEN];
            read(0, s, sizeof(s));
            s[STR_LEN - 1] = '\0';

            int n = (int) strtol(s, NULL, BASE);

            printf("%d %d\n", i, n * n);
            fflush(stdout);

            exit(0);
        }
    }

    while (wait(NULL) != -1) {
    }

    return 0;
}
