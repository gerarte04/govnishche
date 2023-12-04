#include <signal.h>
#include <stdio.h>
#include <unistd.h>

enum
{
    MAX_CNT = 5
};

volatile int n = 0;

void
handler(int sig)
{
    signal(SIGHUP, handler);

    if (n == MAX_CNT) {
        signal(SIGHUP, SIG_DFL);
        _exit(0);
    }

    printf("%d\n", n);
    fflush(stdout);

    n++;

    return;
}

int
main(int argc, char **argv)
{
    signal(SIGHUP, handler);

    printf("%d\n", getpid());
    fflush(stdout);

    for (;;) {
        pause();
    }
}
