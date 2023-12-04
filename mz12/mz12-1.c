#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <unistd.h>

enum
{
    ERR = 42
};

int
main(int argc, char **argv)
{
    pid_t pid = fork();

    if (pid < 0) {
        return -1;
    }

    if (!pid) {
        int fd = open(argv[2], O_RDONLY);

        if (fd < 0 || dup2(fd, 0) < 0) {
            _exit(ERR);
        }

        close(fd);
        fd = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, 0660);

        if (fd < 0 || dup2(fd, 1) < 0) {
            _exit(ERR);
        }

        close(fd);
        fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0660);

        if (fd < 0 || dup2(fd, 2) < 0) {
            _exit(ERR);
        }

        close(fd);
        execlp(argv[1], argv[1], NULL);
        _exit(ERR);
    }

    int st;
    wait(&st);

    printf("%d\n", st);

    return 0;
}
