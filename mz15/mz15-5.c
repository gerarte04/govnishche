#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/signalfd.h>
#include <unistd.h>

enum
{
    FILES_MAXCNT = 20,
    BSIZE = 16
};

int
main(int argc, char **argv)
{
    sigset_t s;
    sigemptyset(&s);
    sigaddset(&s, SIGTERM);

    for (int i = SIGRTMIN; i <= SIGRTMAX; i++) {
        sigaddset(&s, i);
    }

    sigprocmask(SIG_BLOCK, &s, NULL);

    long long sum[FILES_MAXCNT + 1] = {};

    struct pollfd fds[2] = {(struct pollfd){-1, POLLIN, 0}, (struct pollfd){signalfd(-1, &s, 0), POLLIN, 0}};
    struct signalfd_siginfo sinfo = {};
    int nfile = 0;
    int fd_list[FILES_MAXCNT + 1];

    for (int i = 1; i < argc; i++) {
        fd_list[i] = open(argv[i], O_RDONLY);
    }

    printf("%d\n", getpid());
    fflush(stdout);

    while (sinfo.ssi_signo != SIGTERM) {
        poll(fds, 2, -1);

        if (fds[1].revents & POLLIN) {
            read(fds[1].fd, &sinfo, sizeof(sinfo));

            if (sinfo.ssi_signo >= SIGRTMIN && sinfo.ssi_signo <= SIGRTMAX) {
                nfile = sinfo.ssi_signo - SIGRTMIN + 1;
                fds[0].fd = fd_list[nfile];
            }
        } else if (fds[0].revents & POLLIN) {
            char buf[BSIZE + 1];
            buf[BSIZE] = '\0';

            if (read(fds[0].fd, buf, BSIZE) > 0) {
                long long x;
                sscanf(buf, "%lld", &x);
                sum[nfile] += x;
            }
        }
    }

    for (int i = 1; i < argc; i++) {
        close(fd_list[i]);
    }

    for (int i = 1; i < argc; i++) {
        printf("%lld\n", sum[i]);
        fflush(stdout);
    }

    return 0;
}
