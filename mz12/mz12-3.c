#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void wait_all(int *cnt)
{
    int st;
    
    while (wait(&st) != -1) {
        if (WIFEXITED(st) && !WEXITSTATUS(st)) {
            (*cnt)++;
        }
    }
}

int
main(int argc, char **argv)
{
    int cnt = 0;

    for (int i = 1; i < argc; i++) {
        pid_t pid;

        switch (argv[i][0]) {
        case 's':
            wait_all(&cnt);

            if ((pid = fork()) == 0) {
                execlp(&argv[i][1], &argv[i][1], NULL);
                exit(1);
            } else if (pid > 0) {
                wait_all(&cnt);
            }

            break;
        case 'p':
            if ((pid = fork()) == 0) {
                execlp(&argv[i][1], &argv[i][1], NULL);
                exit(1);
            }

            break;
        }
    }

    wait_all(&cnt);
    printf("%d\n", cnt);

    return 0;
}
