#include <signal.h>
#include <stdio.h>
#include <unistd.h>

enum
{
    MODE_ADD,
    MODE_MUL
};

volatile int mode = MODE_ADD;

void
handler_sigint(int sig)
{
    signal(SIGINT, handler_sigint);
    mode = MODE_ADD;
}

void
handler_sigquit(int sig)
{
    signal(SIGQUIT, handler_sigquit);
    mode = MODE_MUL;
}

int
main(int argc, char **argv)
{
    signal(SIGINT, handler_sigint);
    signal(SIGQUIT, handler_sigquit);

    printf("%d\n", getpid());
    fflush(stdout);

    int n;
    int val = 0;

    while (scanf("%d", &n) != EOF) {
        if (mode == MODE_ADD) {
            __builtin_add_overflow(val, n, &val);
        } else if (mode == MODE_MUL) {
            __builtin_mul_overflow(val, n, &val);
        }

        printf("%d\n", val);
        fflush(stdout);
    }

    return 0;
}
