#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

enum
{
    BASE = 10
};

int calc_expr(int val, int nproc)
{
    int part = val % nproc;
    int res = part;

    for (int i = 0; i < 3; i++) {
        res = (res * part) % nproc;
    }

    return res;
}

int
main(int argc, char **argv)
{
    int nproc = strtol(argv[1], NULL, BASE);
    key_t key = strtol(argv[2], NULL, BASE);
    int maxval = strtol(argv[3], NULL, BASE);

    int semid = semget(key, nproc, IPC_CREAT | 0600);

    int *val, *prev;
    int shmid = shmget(key, sizeof(*val) + sizeof(*prev), IPC_CREAT | 0600);
    val = shmat(shmid, NULL, 0);
    prev = val + 1;
    *val = *prev = 0;

    semop(semid, &(struct sembuf) {0, 1, 0}, 1);

    for (int i = 0; i < nproc; i++) {
        if (!fork()) {
            while (semop(semid, &(struct sembuf) {i, -1, 0}, 1) >= 0) {
                printf("%d %d %d\n", i + 1, *val, *prev);
                fflush(stdout);

                if (*val >= maxval) {
                    semctl(semid, 0, IPC_RMID, 0);
                    shmctl(shmid, IPC_RMID, 0);
                    _exit(0);
                }

                (*val)++;
                *prev = i + 1;

                semop(semid, &(struct sembuf) {calc_expr(*val, nproc), 1, 0}, 1);
            }

            _exit(0);
        }
    }

    while (wait(NULL) != -1) {
    }

    return 0;
}
