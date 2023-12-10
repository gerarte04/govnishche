#define _GNU_SOURCE
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

volatile int bit = 0;
volatile int rcvd_bit = 0;
volatile int rcvd_sigalrm = 0;
volatile int rcvd_sigio = 0;

void
handler_sigusr1(int sig)
{
    printf("received sigusr1\n");
    fflush(stdout);
    bit = 0;
    rcvd_bit = 1;
}

void
handler_sigusr2(int sig)
{
    printf("received sigusr2\n");
    fflush(stdout);
    bit = 1;
    rcvd_bit = 1;
}

void
handler_sigalrm(int sig)
{
    printf("received sigalrm\n");
    fflush(stdout);
    rcvd_sigalrm = 1;
}

void
handler_sigio(int sig)
{
    rcvd_sigio = 1;
}

int
main(int argc, char **argv)
{
    sigset_t s;
    sigemptyset(&s);
    sigaddset(&s, SIGUSR1);
    sigaddset(&s, SIGUSR2);
    sigaddset(&s, SIGIO);
    sigaddset(&s, SIGALRM);
    sigprocmask(SIG_BLOCK, &s, NULL);

    sigset_t s1;
    sigemptyset(&s1);

    pid_t pid1 = fork();
    pid_t pid2;

    if (pid1) {
        pid2 = fork();
    }

    if (!pid2) {
        sigaction(SIGALRM, &(struct sigaction) {.sa_handler = handler_sigalrm, .sa_flags = SA_RESTART}, NULL);

        int fd = open(argv[1], O_RDONLY);
        struct stat st;
        stat(argv[1], &st);

        unsigned char *data = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
        close(fd);

        for (int i = 0; i < st.st_size; i++) {
            for (int j = 0; j < CHAR_BIT; j++) {
                if (data[i] >> j & 0x1) {
                    kill(pid1, SIGUSR2);
                } else {
                    kill(pid1, SIGUSR1);
                }

                while (!rcvd_sigalrm) {
                    sigsuspend(&s1);
                }

                rcvd_sigalrm = 0;
            }
        }

        kill(pid1, SIGIO);
        munmap(data, st.st_size);

        _exit(0);
    }

    if (!pid1) {
        sigaction(SIGIO, &(struct sigaction) {.sa_handler = handler_sigio, .sa_flags = SA_RESTART}, NULL);
        sigaction(SIGUSR1, &(struct sigaction) {.sa_handler = handler_sigusr1, .sa_flags = SA_RESTART}, NULL);
        sigaction(SIGUSR2, &(struct sigaction) {.sa_handler = handler_sigusr2, .sa_flags = SA_RESTART}, NULL);

        int cnt = 0;
        char c = 0;

        while (!rcvd_sigio) {
            while (!rcvd_bit && !rcvd_sigio) {
                sigsuspend(&s1);
            }

            if (rcvd_bit) {
                rcvd_bit = 0;
                cnt++;
                c <<= 1;

                if (bit) {
                    c |= 0x1;
                }

                if (cnt == CHAR_BIT) {
                    printf("%c", c);
                    fflush(stdout);
                    cnt = 0;
                }
            }

            kill(0, SIGALRM);
        }

        _exit(0);
    }

    while (wait(NULL) != -1) {
    }

    return 0;
}