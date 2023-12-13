#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

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

void
handler(int sig)
{
    work = 1;
}

int
main(int argc, char **argv)
{
    int x_max = strtol(argv[1], NULL, BASE);

    int fd[2];
    pipe(fd);
    FILE *fr = fdopen(fd[0], "r");
    FILE *fw = fdopen(fd[1], "w");

    sigset_t s1, s2;
    sigemptyset(&s1);
    sigaddset(&s1, SIGUSR1);
    sigprocmask(SIG_BLOCK, &s1, NULL);
    sigemptyset(&s2);

    sigaction(SIGUSR1, &(struct sigaction){.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    pid_t pids[2];

    for (int i = 0; i < 2; i++) {
        if (!(pids[i] = fork())) {
            while (!work) {
                sigsuspend(&s2);
            }

            work = 0;

            int x;
            pid_t other_prc;

            while (fscanf(fr, "%d %d", &x, &other_prc) != -1) {
                if (x == x_max) {
                    fprintf(fw, "%d %d\n", x, getpid());
                    fflush(fw);

                    fclose(fr);
                    fclose(fw);

                    kill(other_prc, SIGUSR1);
                    _exit(0);
                }

                printf("%d %d\n", i + 1, x);
                fflush(stdout);

                x++;

                fprintf(fw, "%d %d\n", x, getpid());
                fflush(fw);

                kill(other_prc, SIGUSR1);

                while (!work) {
                    sigsuspend(&s2);
                }

                work = 0;
            }

            _exit(0);
        }
    }

    fprintf(fw, "1 %d\n", pids[1]);
    fclose(fr);
    fclose(fw);

    kill(pids[0], SIGUSR1);

    while (wait(NULL) != -1) {
    }

    printf("Done\n");
    fflush(stdout);

    return 0;
}
