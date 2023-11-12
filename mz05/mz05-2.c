#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

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
        int mode_len = (sizeof(str) - 1) / sizeof(str[0]);

        for (int i = 0; i < mode_len; i++) {
            if (!(mode & 1)) {
                str[mode_len - i - 1] = '-';
            }

            mode >>= 1;
        }

        printf("%s\n", str);
    }

    return 0;
}
