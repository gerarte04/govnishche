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
    sigaddset(&s, SIGRTMIN);
    sigprocmask(SIG_BLOCK, &s, NULL);

    long long sum[FILES_MAXCNT + 1] = {};
    int nfile = 0;

    struct pollfd fds[2] = {(struct pollfd){-1, POLLIN, 0}, (struct pollfd){signalfd(-1, &s, 0), POLLIN, 0}};
    struct signalfd_siginfo sinfo = {};

    printf("%d\n", getpid());
    fflush(stdout);

    while (sinfo.ssi_signo != SIGTERM) {
        poll(fds, 2, -1);

        if (fds[1].revents & POLLIN) {
            read(fds[1].fd, &sinfo, sizeof(sinfo));

            if (sinfo.ssi_signo == SIGRTMIN) {
                close(fds[0].fd);

                if (nfile < argc - 1 && nfile < FILES_MAXCNT) {
                    nfile++;
                    fds[0].fd = open(argv[nfile], O_RDONLY);
                }
            }
        } else if (fds[0].revents & POLLIN) {
            char buf[BSIZE + 1];
            buf[BSIZE] = '\0';

            if (read(fds[0].fd, buf, BSIZE) == 0) {
                close(fds[0].fd);
                continue;
            }

            long long x;
            sscanf(buf, "%lld", &x);
            sum[nfile] += x;
        }
    }

    close(fds[0].fd);

    for (int i = 1; i < argc; i++) {
        printf("%lld\n", sum[i]);
        fflush(stdout);
    }

    return 0;
}
