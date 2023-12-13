#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/signalfd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
    pid_t pid1 = fork();

    if (pid1 && !fork()) {
        sigset_t s;
        sigemptyset(&s);
        sigaddset(&s, SIGALRM);
        sigprocmask(SIG_BLOCK, &s, NULL);

        int sfd = signalfd(-1, &s, 0);
        struct signalfd_siginfo sinfo;

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

                read(sfd, &sinfo, sizeof(sinfo));
            }
        }

        kill(pid1, SIGIO);
        munmap(data, st.st_size);

        _exit(0);
    }

    if (!pid1) {
        sigset_t s;
        sigemptyset(&s);
        sigaddset(&s, SIGUSR1);
        sigaddset(&s, SIGUSR2);
        sigaddset(&s, SIGIO);
        sigprocmask(SIG_BLOCK, &s, NULL);

        int sfd = signalfd(-1, &s, 0);
        struct signalfd_siginfo sinfo;
        read(sfd, &sinfo, sizeof(sinfo));

        int cnt = 0;
        char c = 0;

        while (sinfo.ssi_signo != SIGIO) {
            if (sinfo.ssi_signo == SIGUSR2) {
                c |= 0x1 << cnt;
            }

            cnt++;

            if (cnt == CHAR_BIT) {
                printf("%c", c);
                fflush(stdout);
                cnt = 0;
                c = 0;
            }

            kill(sinfo.ssi_pid, SIGALRM);
            read(sfd, &sinfo, sizeof(sinfo));
        }

        _exit(0);
    }

    while (wait(NULL) != -1) {
    }

    return 0;
}
