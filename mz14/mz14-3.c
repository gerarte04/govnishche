#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <unistd.h>

enum
{
    ARG_OFF = 2,
    BASE = 10
};

int
runexec(char *file)
{
    char name[PATH_MAX];

    int fd = open(file, O_RDONLY);
    read(fd, name, PATH_MAX * sizeof(name[0]));
    close(fd);
    name[strlen(name) - 1] = '\0';

    if (!fork()) {
        execlp(name, name, NULL);
        exit(1);
    }
}

int
main(int argc, char **argv)
{
    int n = strtol(argv[1], NULL, BASE) + ARG_OFF;
    n = (n > argc) ? argc : n;

    int cnt = 0;

    for (int i = ARG_OFF; i < n; i++) {
        runexec(argv[i]);
    }

    int st;

    while (wait(&st) != -1) {
        if (WIFEXITED(st) && !WEXITSTATUS(st)) {
            cnt++;
        }
    }

    for (int i = n; i < argc; i++) {
        runexec(argv[i]);
        wait(&st);

        if (WIFEXITED(st) && !WEXITSTATUS(st)) {
            cnt++;
        }
    }

    printf("%d\n", cnt);
    fflush(stdout);

    return 0;
}
