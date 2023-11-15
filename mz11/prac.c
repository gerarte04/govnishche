#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <unistd.h>

int f1(int argc, char **argv)
{
    int fd[2];
    pipe(fd);

    if (!fork()) {
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);
        execlp(argv[1], argv[1], NULL);
    }

    if (!fork()) {
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);
        execlp(argv[2], argv[2], NULL);
    }

    close(fd[0]);
    close(fd[1]);

    while (wait(NULL) != -1) {
    }

    return 0;
}

void ping(int fdr, int fdw, int x_max, int n_prc)
{
    int rst;
    int x;

    while ((rst = read(fdr, &x, sizeof(x))) != -1 && rst != 0) {
        printf("prc %d: %d\n", n_prc, x);
        fflush(stdout);

        if (x == x_max) {
            close(fdr);
            close(fdw);
            exit(0);
        }

        x++;

        write(fdw, &x, sizeof(x));
    }
}

int main(int argc, char **argv)
{
    int x_max = strtol(argv[1], NULL, 10);

    int fd1[2];
    pipe(fd1);
    int fd2[2];
    pipe(fd2);

    int x = 1;
    write(fd1[1], &x, sizeof(x));

    if (!fork()) {
        close(fd2[0]);
        close(fd1[1]);
        ping(fd1[0], fd2[1], x_max, 1);
    }

    if (!fork()) {
        close(fd2[1]);
        close(fd1[0]);
        ping(fd2[0], fd1[1], x_max, 2);
    }

    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);
    close(fd1[0]);

    while (wait(NULL) != -1) {
        
    }

    return 0;
}