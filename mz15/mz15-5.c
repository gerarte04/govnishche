#define _GNU_SOURCE
#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <unistd.h>

enum
{
    BSIZE = 16
};

volatile int nfile = 1;
volatile int rcvd_rfile = 0;
volatile int rcvd_pnums = 0;

void
handler_sigterm(int sig)
{
    rcvd_pnums = 1;
}

void
handler_sigrtmin(int sig)
{
    nfile++;
    rcvd_rfile = 1;
}

int
main(int argc, char **argv)
{
    sigset_t s;
    sigemptyset(&s);
    sigaddset(&s, SIGTERM);
    sigaddset(&s, SIGRTMIN);
    sigprocmask(SIG_BLOCK, &s, NULL);

    sigset_t s1;
    sigemptyset(&s1);

    sigaction(SIGTERM, &(struct sigaction) {.sa_handler = handler_sigterm, .sa_flags = SA_RESTART}, NULL);
    sigaction(SIGRTMIN, &(struct sigaction) {.sa_handler = handler_sigrtmin, .sa_flags = SA_RESTART}, NULL);

    long long sum[BSIZE] = {};
    int fd = open(argv[1], O_RDONLY);

    printf("%d\n", getpid());

    while (!rcvd_pnums) {
        int status = ppoll(&(struct pollfd) { fd, POLLIN, 0 }, 1, NULL, &s1);

        if (rcvd_rfile) {
            close(fd);
            fd = open(argv[nfile], O_RDONLY);
            rcvd_rfile = 0;
        }

        if (!rcvd_pnums) {
            char buf[BSIZE + 1];
            read(fd, buf, BSIZE);
            buf[BSIZE] = '\0';

            long long x;
            sscanf(buf, "%lld", &x);
            sum[nfile] += x;
        }
    }

    for (int i = 1; i < argc; i++) {
        printf("%lld\n", sum[i]);
    }

    return 0;
}
