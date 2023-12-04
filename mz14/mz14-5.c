#include <signal.h>
#include <stdio.h>
#include <unistd.h>

enum
{
    MAX_CNT = 3
};

volatile int p = 0;
volatile int n = 0;

void
handler_sigint(int sig)
{
    signal(sig, handler_sigint);

    if (n == MAX_CNT) {
        signal(sig, SIG_DFL);
        _exit(0);
    }

    printf("%d\n", p);
    fflush(stdout);

    n++;
}

void
handler_sigterm(int sig)
{
    _exit(0);
}

int
is_prime(int n)
{
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }

    return 1;
}

int
main(int argc, char **argv)
{
    signal(SIGINT, handler_sigint);
    signal(SIGTERM, handler_sigterm);

    int low;
    int high;
    scanf("%d %d", &low, &high);

    printf("%d\n", getpid());
    fflush(stdout);

    for (int i = low; i < high; i++) {
        if (is_prime(i)) {
            p = i;
        }
    }

    printf("-1\n");
    fflush(stdout);

    return 0;
}
