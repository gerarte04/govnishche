#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void f1(int argc, char **argv){
    long num = 0;

    for (int i = 1; i < argc; i++) {
        char *buf;
        errno = 0;
        
        long a = strtol(argv[i], &buf, 0);

        if (*buf || errno || buf == argv[i] || (int)a != a) {
            continue;
        }

        num += a;
    }

    printf("%d\n", num);
}

void f2(void) {
    char str[80];

    while (fgets(str, 80, stdin)) {
        double sum = 0;
        double n;
        int cnt = 0;
        int c;

        while (sscanf(str + cnt, "%lf%n", &n, &c)) {
            sum += n;
            cnt += c;
        }

        printf("%lf\n", sum);
    }
}

int f3(int a) {
    int n = 0;
    unsigned int b = a;

    while (b) {
        if (b & 1) {
            ++n;
        }

        b = b >> 1;
    }

    return n;
}

int main(int argc, char **argv) {
    printf("%d", f3(-1));

    return 0;
}