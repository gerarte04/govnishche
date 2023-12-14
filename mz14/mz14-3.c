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

void
runexec(char *file)
{
    char name[PATH_MAX];
    FILE *f;

    if ((f = fopen(file, "r")) == NULL || fgets(name, PATH_MAX, f) == NULL) {
        return;
    }

    fclose(f);
    name[strlen(name) - 1] = '\0';

    if (!fork()) {
        execlp(name, name, NULL);
        _exit(1);
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
