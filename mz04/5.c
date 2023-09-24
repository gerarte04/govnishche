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

    unsigned char c;

    for (int i = 0; read(fr, &c, sizeof(c)) > 0; i++) {
        int off = i * sizeof(c);

        for (int j = 1; j <= CHAR_BIT; j++) {
            int x = j + off;
            sum += x * x;

            if (c & 1) {
                int wr = sum % mod;
                write(fw, &wr, sizeof(wr));
            }

            c >>= 1;
        }
    }

    close(fr);
    close(fw);

    return 0;
}
