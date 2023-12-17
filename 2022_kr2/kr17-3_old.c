#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int *n;
    int shmid = shmget(IPC_PRIVATE, sizeof(*n), IPC_CREAT | 0600);
    n = shmat(shmid, NULL, 0);
    setbuf(stdin, 0);
    scanf("%d", n);
    printf("%d %d\n", getpid(), *n);
    fflush(stdout);

    while (*n > 0) {
        for (int i = 0; i < *n; i++) {
            pid_t pid = fork();

            if (!pid) {
                if (i == *n - 1) {
                    if (scanf("%d", n) == EOF) {
                        *n = 0;
                        _exit(0);
                    }
                    
                    printf("%d %d\n", getpid(), *n);
                    fflush(stdout);
                    _exit(0);
                }
            } else {
                wait(NULL);

                if (i != 0) {
                    _exit(0);
                } else {
                    break;
                }
            }
        }
    }

    shmctl(shmid, IPC_RMID, NULL);
    
    return 0;
}
