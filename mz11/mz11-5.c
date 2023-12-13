#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
    int n;
    int is_parent = 1;

    while (scanf("%d", &n) != EOF) {
        pid_t pid = fork();

        if (pid < 0) {
            printf("-1\n");
            fflush(stdout);
            _exit(1);
        }

        if (pid) {
            int st;
            wait(&st);

            if (!WIFEXITED(st) || WEXITSTATUS(st)) {
                if (is_parent) {
                    _exit(0);
                } else {
                    _exit(1);
                }
            }

            printf("%d\n", n);
            fflush(stdout);

            _exit(0);
        }

        is_parent = 0;
    }

    return 0;
}
