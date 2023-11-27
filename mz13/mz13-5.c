#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <unistd.h>

enum
{
    BASE = 10
};

void
ping(int *fd1, int *fd2, int x_max, int n_prc)
{
    close(fd1[1]);
    close(fd2[0]);

    FILE *fr = fdopen(fd1[0], "r");
    FILE *fw = fdopen(fd2[1], "w");

    int x;

    while (fscanf(fr, "%d", &x) != -1) {
        if (x == x_max) {
            fclose(fr);
            fclose(fw);
            exit(0);
        }

        printf("%d %d\n", n_prc, x);
        fflush(stdout);

        x++;

        fprintf(fw, "%d\n", x);
        fflush(fw);
    }

    exit(0);
}

int
main(int argc, char **argv)
{
    int x_max = strtol(argv[1], NULL, BASE);

    int fd1[2];
    pipe(fd1);
    int fd2[2];
    pipe(fd2);

    if (!fork()) {
        ping(fd1, fd2, x_max, 1);
    }

    if (!fork()) {
        ping(fd2, fd1, x_max, 2);
    }

    dprintf(fd1[1], "1\n");

    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);
    close(fd1[0]);

    while (wait(NULL) != -1) {
    }

    printf("Done\n");
    fflush(stdout);

    return 0;
}
