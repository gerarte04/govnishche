#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <unistd.h>

// не тестил

int main(int argc, char **argv)
{
    if (!fork()) {
        int fd[2];
        pipe(fd);

        if (!fork()) {
            int fin = open(argv[2], O_RDONLY);
            dup2(fin, 0);
            dup2(fd[1], 1);
            
            close(fd[0]);
            close(fd[1]);
            close(fin);

            execlp(argv[1], argv[1], NULL);
            _exit(1);
        }

        if (!fork()) {
            dup2(fd[0], 0);
            close(fd[0]);
            close(fd[1]);

            execlp(argv[3], argv[3], NULL);
            _exit(1);
        }

        close(fd[0]);
        close(fd[1]);

        int flag = 0;
        int st;

        while (wait(&st) != -1) {
            if (!WIFEXITED(st) || WEXITSTATUS(st)) {
                flag = 1;
            }
        }

        _exit(flag);
    }

    int st;
    wait(&st);

    if (WIFEXITED(st) && !WEXITSTATUS(st)) {
        if (!fork()) {
            execlp(argv[4], argv[4], NULL);
            _exit(1);
        }
    }

    wait(NULL);

    return 0;
}
