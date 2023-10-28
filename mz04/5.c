#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
    char *buf = NULL;
    errno = 0;
    int sum = 0;
    long m = strtol(argv[3], &buf, 10);

    if (errno || *buf || buf == argv[3] || (int) m != m || m == 0) {
        return 0;
    }

    int mod = (int) m;
    int fr = open(argv[1], O_RDONLY);
    int fw = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);

    unsigned char c;
    long long x = 0;

    for (int i = 0; read(fr, &c, sizeof(c)) == sizeof(c); i++) {
        for (int j = 1; j <= CHAR_BIT; j++) {
            x++;
            sum = (sum % mod + (x * x) % mod) % mod;

            if (c & 1) {
                if (write(fw, &sum, sizeof(sum)) != sizeof(sum)) {
                    return 1;
                }
            }

            c >>= 1;
        }

        c = 0;
    }

    close(fr);
    close(fw);

    return 0;
}
