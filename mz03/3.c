#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int
main(int argc, char **argv)
{
    char *buf = NULL;
    errno = 0;

    double course = round(strtod(argv[1], &buf) * 10000);

    if (*buf || errno || buf == argv[1]) {
        return 1;
    }

    for (int i = 2; i < argc; i++) {
        buf = NULL;
        errno = 0;

        double a = strtod(argv[i], &buf);

        if (*buf || errno || buf == argv[i]) {
            return 1;
        }

        course += course * a / 100;
        course = round(course);
    }

    printf("%0.4lf\n", course / 10000);

    return 0;
}
