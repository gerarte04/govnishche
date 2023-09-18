#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int
main(int argc, char **argv)
{
    char *buf = NULL;
    errno = 0;

    double course = strtod(argv[1], &buf);

    if (*buf || errno || buf == argv[1] || (double) course != course) {
        return 0;
    }

    for (int i = 2; i < argc; i++) {
        buf = NULL;
        errno = 0;

        double a = strtod(argv[i], &buf);

        if (*buf || errno || buf == argv[i] || (double) a != a) {
            continue;
        }

        course += round(course * a * 100) / 10000;
    }

    printf("%0.4lf\n", course);

    return 0;
}
