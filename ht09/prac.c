#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <unistd.h>

int f1(int argc, char **argv)
{
    char *buf;
    int n = (int) strtol(argv[2], &buf, 10);

    pid_t pid;

    truncate(argv[1], 0);

    for (int i = 1; i <= n; i++) {
        pid = fork();

        if (pid == 0) {
            int fd = open(argv[1], O_CREAT | O_WRONLY, 0644);
            lseek(fd, 0, SEEK_END);
            write(fd, &i, sizeof(i));
            close(fd);

            return 0;
        }
    }

    while (wait(NULL) != -1) {
        
    }

    return 0;
}

int forkexec(char **argv, int n)
{
    pid_t pid = fork();

    if (pid == 0) {
        execlp(argv[n], argv[n], NULL);
    }

    int st;
    wait(&st);

    return WIFEXITED(st) && !WEXITSTATUS(st);
}

int f2(int argc, char **argv)
{
    return !((forkexec(argv, 1) || forkexec(argv, 2)) && forkexec(argv, 3));
}

int f3(int argc, char **argv)
{
    pid_t pid = getpid();

    for (int i = 1; i <= 3; i++) {
        pid = fork();

        if (!pid) {
            char buf[8];
            setbuf(stdin, 0);
            fgets(buf, 7, stdin);

            char *eptr = NULL;
            int n = strtol(buf, &eptr, 10);

            printf("%d %d\n", n * n, i);

            return 0;
        }
    }

    while (wait(NULL) != -1) {
        
    }

    return 0;
}

int main(int argc, char **argv)
{
    return f3(argc, argv);
}