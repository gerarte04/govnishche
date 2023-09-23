#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum
{
    ROUND_COEF = 10000, // на коэфф-т домножается курс для того, чтобы округлять его при помощи round
    PERCENT = 100       // для перевода из процентов в доли
};

int
main(int argc, char **argv)
{
    char *buf = NULL;
    errno = 0;

    double course = round(strtod(argv[1], &buf) * ROUND_COEF);

    if (*buf || errno || buf == argv[1]) {
        return 1;
    }

    for (int i = 2; i < argc; i++) {
        buf = NULL;
        errno = 0;

        double change = strtod(argv[i], &buf);

        if (*buf || errno || buf == argv[i]) {
            return 1;
        }

        course += course * change / PERCENT;
        course = round(course);
    }

    printf("%0.4lf\n", course / ROUND_COEF);

    return 0;
}
