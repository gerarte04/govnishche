#include <stdlib.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int
main(int argc, char **argv)
{
    long long n_min;
    int off_min = 0;
    long long n;
    int off = 0;

    int f;

    if ((f = open(argv[1], O_RDWR)) == -1) {
        return 0;
    }
    
    if (read(f, &n_min, sizeof(n_min)) <= 0) {
        fclose(f);
        return 0;
    }

    while (read(f, &n, sizeof(n)) > 0) {
        off++;
        
        if (n < n_min) {
            n_min = n;
            off_min = off;
        }
    }

    n_min = -n_min;
    lseek(f, off_min * sizeof(n_min), SEEK_SET);
    write(f, &n_min, sizeof(n_min));
    close(f);

    return 0;
}
