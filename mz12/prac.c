#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <unistd.h>

int f1(int argc, char **argv)
{
    int fd[2];
    pipe(fd);

    if (!fork()) {
        close(fd[0]);
        int fin = open(argv[4], O_RDONLY);
        dup2(fin, 0);
        close(fin);
        dup2(fd[1], 1);
        close(fd[1]);

        execlp(argv[1], argv[1], NULL);
    }

    int st;
    wait(&st);

    if (WIFEXITED(st) && !WEXITSTATUS(st)) {
        if (!fork()) {
            dup2(fd[1], 1);
            close(fd[1]);
            dup2(fd[0], 0);
            close(fd[0]);

            execlp(argv[2], argv[2], NULL);
        }

        wait(NULL);
    }

    if (!fork()) {
        close(fd[1]);
        int fout = open(argv[5], O_CREAT | O_APPEND | O_WRONLY);
        dup2(fout, 1);
        close(fout);
        dup2(fd[0], 0);
        close(fd[0]);

        execlp(argv[3], argv[3], NULL);
    }

    wait(NULL);

    close(fd[0]);
    close(fd[1]);

    return 0;
}

// SIGINT - ctrl + c
// SIGKILL - убить процесс (плохо)
// SIGTERM - завершить с выгрузкой буферов и т. п.
// SIGALRM - сигнал, посланный ф-цией alarm (будильник)
// SIGCHLD - при завершении сына
// SIGPIPE - когда все дескрипторы пайпа закрыты / произошел обрыв соединения
// SIGUSR1, SIGUSR2 - для пользовательского взаим-я пр-сов


volatile int n = 0;

void handler1(int sig)
{
    n++;

    if (n == 4) {
        signal(sig, SIG_DFL);
    }
}

int f2(int argc, char **argv)
{
    int n = 0;

    signal(SIGINT, handler1);
    
    for (;;) {
        pause();
    }
}

volatile int mode = 0;

void handler2(int sig)
{
    mode = 0;
}

void handler3(int sig)
{
    mode = 1;
}

int main(int argc, char **argv)
{
    signal(SIGUSR1, handler2);
    signal(SIGUSR2, handler3);

    int n;
    
    while (scanf("%d", &n) != EOF) {
        if (mode) {
            printf("%d\n", n * n);
        } else {
            printf("%d\n", -n);
        }
    }

    return 0;
}
