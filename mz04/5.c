#include <stdlib.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>

enum
{
    CHAR_BIT = 8
};

int
main(int argc, char **argv)
{
    char *buf = NULL;
    int sum = 0;
    int mod = (int) strtol(argv[3], &buf, 10);

    int fr = open(argv[1], O_RDONLY);
    int fw = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0777);

    if (fr == -1 || fw == -1) {
        close(fr);
        close(fw);
        return 0;
    }

    unsigned char c;

    for (int i = 0; read(fr, &c, sizeof(c)) > 0; i++) {
        int off = i * sizeof(c);

        for (int j = 1; j <= CHAR_BIT; j++) {
            int x = j + off;
            sum = (sum % mod + (x * x) % mod) % mod;

            if (c & 1) {
                int k = write(fw, &sum, sizeof(sum));

                if (k != sizeof(sum)) {
                    write(fw, &sum + k, sizeof(sum) - k);
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
