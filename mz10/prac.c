#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <unistd.h>

void f1(int argc, char **argv)
{
    char *new_argv[] = { "ls", "-al", NULL };
    execvp("ls", new_argv);
    exit(1);
}

int f2(int argc, char **argv) // последовательно
{
    int cnt = 0;

    for (int i = 1; i < argc; i++) {
        pid_t pid;

        if (!(pid = fork())) {
            execlp(argv[i], argv[i], NULL);
            exit(1);
        }

        int status;
        wait(&status);

        if (!WIFEXITED(status) || WEXITSTATUS(status)) {
            cnt++;
        }
    }

    printf("%d\n", cnt);

    return 0;
}

int f3(int argc, char **argv)   // параллельно
{
    int cnt = 0;

    for (int i = 1; i < argc; i++) {
        pid_t pid;

        if (!(pid = fork())) {
            execlp(argv[i], argv[i], NULL);
            exit(1);
        }
    }

    int status;

    while (wait(&status) != -1) {
        if (!WIFEXITED(status) || WEXITSTATUS(status)) {
            cnt++;
        }
    }

    printf("%d\n", cnt);

    return 0;
}

int f4(int argc, char **argv)
{
    char *buf;
    int n = (int) strtol(argv[1], &buf, 10);

    for (int i = 1; i <= n; i++) {
        printf("%d\n", i);
        fflush(stdout);
        
        if (i < n) {
            pid_t pid = fork();

            if (pid < 0) {
                exit(1);
            }

            if (pid) {
                wait(NULL);

                exit(0);
            }
        }
    }
}

int proc(void)
{
    pid_t pid = fork();

    if (!pid) {
        printf("1\n");
    }

    return pid;
}

int main(int argc, char **argv)
{
    proc(), proc(), proc();
}