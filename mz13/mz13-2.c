#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
    int fd[2];
    pipe(fd);

    if (!fork()) {
        if (!fork()) {
            int n;
            int sum = 0;

            int rst;
            close(fd[1]);

            while ((rst = read(fd[0], &n, sizeof(n))) != 0 && rst != -1) {
                sum += n;
            }

            close(fd[0]);

            printf("%d\n", sum);
            exit(0);
        }

        close(fd[0]);
        close(fd[1]);
        wait(NULL);
        exit(0);
    }

    close(fd[0]);
    int n;

    while (scanf("%d", &n) != EOF) {
        write(fd[1], &n, sizeof(n));
    }

    close(fd[1]);
    wait(NULL);

    return 0;
}
