#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int
forkexec(char *cmd)
{
    pid_t pid = fork();

    if (pid < 0) {
        return 0;
    }

    if (pid == 0) {
        execlp(cmd, cmd, NULL);
    }

    int st;
    wait(&st);

    return WIFEXITED(st) && !WEXITSTATUS(st);
}

int
main(int argc, char **argv)
{
    return !((forkexec(argv[1]) || forkexec(argv[2])) && forkexec(argv[3]));
}
