#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

enum
{
    MODE_LEN = 9
};

int
main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++) {
        char *buf = NULL;
        errno = 0;
        unsigned int mode = (unsigned int) strtoul(argv[i], &buf, 8);

        if (*buf || errno || buf == argv[i]) {
            continue;
        }

        char str[] = "rwxrwxrwx";

        for (int i = 0; i < MODE_LEN; i++) {
            if (!(mode & 1)) {
                str[MODE_LEN - i - 1] = '-';
            }

            mode >>= 1;
        }

        printf("%s\n", str);
    }

    return 0;
}
