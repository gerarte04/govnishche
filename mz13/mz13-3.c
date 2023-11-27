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
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);

        if (!fork()) {
            int fin = open(argv[4], O_RDONLY);
            dup2(fin, 0);
            close(fin);

            execlp(argv[1], argv[1], NULL);
            exit(1);
        }

        int st;
        wait(&st);

        if (WIFEXITED(st) && !WEXITSTATUS(st)) {
            if (!fork()) {
                execlp(argv[2], argv[2], NULL);
                exit(1);
            }

            wait(NULL);
        }

        exit(0);
    }

    if (!fork()) {
        int fout = open(argv[5], O_CREAT | O_WRONLY | O_APPEND, 0700);
        dup2(fout, 1);
        dup2(fd[0], 0);

        close(fd[0]);
        close(fd[1]);
        close(fout);

        execlp(argv[3], argv[3], NULL);
        exit(1);
    }

    close(fd[0]);
    close(fd[1]);

    while (wait(NULL) != -1) {
    }

    return 0;
}
