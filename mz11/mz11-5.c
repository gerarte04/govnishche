#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
    pid_t pid = fork();

    if (pid < 0) {
        printf("-1\n");
        return 0;
    }

    if (pid) {
        wait(NULL);
        return 0;
    }

    int n;

    while (scanf("%d", &n) != EOF) {
        pid_t pid = fork();

        if (pid < 0) {
            printf("-1\n");
            fflush(stdout);
            exit(1);
        }

        if (pid) {
            int st;
            wait(&st);

            if (!WIFEXITED(st) || WEXITSTATUS(st)) {
                exit(1);
            }

            printf("%d\n", n);
            fflush(stdout);

            exit(0);
        }
    }

    return 0;
}
