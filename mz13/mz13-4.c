#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

pid_t *sons;
int i;

void
slaughter(int sig)
{
    for (int j = 1; j < i; j++) {
        kill(sons[j], SIGKILL);
    }

    while (wait(NULL) != -1) {
    }

    _exit(1);
}

int
main(int argc, char **argv)
{
    pid_t sons_arr[argc];
    sons = sons_arr;
    signal(SIGUSR1, slaughter);

    close(2);

    int fd_prev = 0;

    for (i = 1; i < argc; i++) {
        int fd[2];

        if (pipe(fd) != 0 || (sons[i] = fork()) < 0) {
            slaughter(SIGUSR1);
        }

        if (!sons[i]) {
            if (i != 1) {
                if (dup2(fd_prev, 0) < 0) {
                    kill(getppid(), SIGUSR1);
                }
            }

            if (i != argc - 1) {
                if (dup2(fd[1], 1) < 0) {
                    kill(getppid(), SIGUSR1);
                }
            }

            close(fd[0]);
            close(fd[1]);

            if (fd_prev) {
                close(fd_prev);
            }

            execlp(argv[i], argv[i], NULL);
            kill(getppid(), SIGUSR1);
        }

        close(fd[1]);

        if (fd_prev) {
            close(fd_prev);
        }

        fd_prev = fd[0];
    }

    while (wait(NULL) != -1) {
    }

    return 0;
}
