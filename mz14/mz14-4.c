#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <unistd.h>

enum
{
    BASE = 10
};

volatile int work = 0;

void handler(int sig)
{
    work = 1;
}

void
ping(int *fd, int x_max, int n_prc)
{
    while (!work) {
        pause();
    }

    FILE *fr = fdopen(fd[0], "r");
    FILE *fw = fdopen(fd[1], "w");

    int x;
    pid_t other_prc;

    while (fscanf(fr, "%d %d", &x, &other_prc) != -1) {
        if (x == x_max) {
            fprintf(fw, "%d %d\n", x, getpid());
            fflush(fw);

            fclose(fr);
            fclose(fw);

            kill(other_prc, SIGUSR1);
            exit(0);
        }

        printf("%d %d\n", n_prc, x);
        fflush(stdout);

        x++;

        fprintf(fw, "%d %d\n", x, getpid());
        fflush(fw);

        work = 0;
        kill(other_prc, SIGUSR1);

        while (!work) {
            pause();
        }
    }

    fclose(fr);
    fclose(fw);
    exit(0);
}

int
main(int argc, char **argv)
{
    int x_max = strtol(argv[1], NULL, BASE);

    int fd[2];
    pipe(fd);

    pid_t pid1, pid2;
    sigaction(SIGUSR1, &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART }, NULL);

    if (!(pid1 = fork())) {
        ping(fd, x_max, 1);
    }

    if (!(pid2 = fork())) {
        ping(fd, x_max, 2);
    }

    dprintf(fd[1], "1 %d\n", pid2);
    close(fd[0]);
    close(fd[1]);

    kill(pid1, SIGUSR1);

    while (wait(NULL) != -1) {
    }

    printf("Done\n");
    fflush(stdout);

    return 0;
}
