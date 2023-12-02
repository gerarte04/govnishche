#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum
{
    MAX_CNT = 5
};

volatile int n = 0;

void
handler(int sig)
{
    if (n == MAX_CNT) {
        exit(0);
    }

    printf("%d\n", n);
    fflush(stdout);

    n++;
}

int
main(int argc, char **argv)
{
    printf("%d\n", getpid());
    fflush(stdout);

    sigaction(SIGHUP, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);

    for (;;) {
        pause();
    }
}