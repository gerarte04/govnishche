#include <stdlib.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <unistd.h>

int runcmd(char *cmd, int *fd, char *input, char *output)
{
    pid_t pid = fork();

    if (pid < 0) {
        return 0;
    }

    if (pid == 0) {
        if (input != NULL) {
            int fin = open(input, O_RDONLY);
            dup2(fin, 0);
            close(fin);
        } else {
            dup2(fd[0], 0);
        }

        if (output != NULL) {
            int fout = open(output, O_CREAT | O_WRONLY | O_APPEND, 0644);
            dup2(fout, 1);
            close(fout);
        } else {
            dup2(fd[1], 1);
        }

        close(fd[0]);
        close(fd[1]);

        execlp(cmd, cmd, NULL);
    }

    int st;
    wait(&st);

    return WIFEXITED(st) && !WEXITSTATUS(st);
}

int
main(int argc, char **argv)
{
    int fd[2];
    pipe(fd);

    if (runcmd(argv[1], fd, argv[4], NULL)) {
        runcmd(argv[2], fd, NULL, NULL);
    }

    runcmd(argv[3], fd, NULL, argv[5]);

    close(fd[0]);
    close(fd[1]);

    return 0;
}
