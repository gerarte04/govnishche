#include <stdio.h>

enum
{
    MIN_N = 2,
    MAX_N = 1999
};

int
bin_exp(int a, int p, int n)
{
    int res = 1;

    while (p != 0) {
        if (p & 1) {
            res = res * a % n;
        }

        a = a * a % n;
        p >>= 1;
    }

    return res;
}

int
is_prime(int a)
{
    for (int i = 2; i * i <= a; i++) {
        if (a % i == 0) {
            return 0;
        }
    }

    return 1;
}

int
main(void)
{
    int n;

    if (scanf("%d", &n) == EOF) {
        printf("error: input value is not read\n");
        return 0;
    }

    if (n < MIN_N || n > MAX_N || !is_prime(n)) {
        printf("error: incorrect input value\n");
        return 0;
    }

    for (int c = 0; c < n; c++) {
        for (int a = 1; a < n; a++) {
            printf("%d ", c * bin_exp(a, n - 2, n) % n);
        }

        printf("\n");
    }

    return 0;
}