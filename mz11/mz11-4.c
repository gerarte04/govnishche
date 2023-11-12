#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
    int n;
    scanf("%d", &n);

    for (int i = 1; i <= n; i++) {
        char *fmt = (i == n) ? "%d\n" : "%d ";
        printf(fmt, i);
        fflush(stdout);

        if (i < n) {
            pid_t pid = fork();

            if (pid < 0) {
                exit(1);
            }

            if (pid) {
                wait(NULL);

                exit(0);
            }
        }
    }

    return 0;
}
