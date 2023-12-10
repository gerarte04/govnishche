#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>

enum
{
    BASE = 10
};

int
main(int argc, char **argv)
{
    int nproc = strtol(argv[1], NULL, BASE);
    int semid = semget(IPC_PRIVATE, nproc, IPC_CREAT | 0600);
    semop(semid, &(struct sembuf) {0, 1, 0}, 1);

    setbuf(stdin, 0);

    for (int i = 0; i < nproc; i++) {
        if (!fork()) {
            while (semop(semid, &(struct sembuf) {i, -1, 0}, 1) >= 0) {
                int n;

                if (scanf("%d", &n) == EOF) {
                    semctl(semid, 0, IPC_RMID, 0);
                    _exit(0);
                }

                printf("%d %d\n", i, n);
                fflush(stdout);

                semop(semid, &(struct sembuf) {n % nproc, 1, 0}, 1);
            }

            _exit(0);
        }
    }

    while (wait(NULL) != -1) {
    }

    return 0;
}
