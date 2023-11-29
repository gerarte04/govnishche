#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

// while (!semctl(id, 0, GETVAL, 0)) {
//     semctl(id, 0, SETVAL, 0);  - так работать нельзя (операция не атомарная)
// }

// struct sembuf p = { 0, -1, 0 };
// struct sembuf v = { 0, 1, 0 };

// semop(id, &p, 1); - захват ресурса
// semop(id, &v, 1); - освобождение ресурса

int f1(int argc, char **argv)
{
    struct sembuf p = { 0, -1, 0 }, v = { 0, 1, 0 };
    struct sembuf p1 = { 1, -1, 0 }, v1 = { 1, 1, 0 };

    int semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0600);
    int shmid = shmget(IPC_PRIVATE, 256, IPC_CREAT | 0600);

    if (!fork()) {
        char *mem = shmat(shmid, NULL, 0);

        while (fgets(mem, 256, stdin)) {
            semop(semid, &v, 1);
            semop(semid, &p1, 1);
        }

        semctl(semid, 0, IPC_RMID, 0);
        shmctl(shmid, IPC_RMID, 0);

        exit(0);
    }

    if (!fork()) {
        char *mem = shmat(shmid, NULL, 0);

        while (semop(semid, &p, 1) >= 0) {
            printf("%s", mem);
            semop(semid, &v1, 1);
        }

        exit(0);
    }

    while (wait(NULL) != -1) {
        
    }

    return 0;
}

int main(int argc, char **argv) // не работает
{
    int nproc = strtol(argv[1], NULL, 10);
    int xmax = strtol(argv[2], NULL, 10);

    int semid = semget(IPC_PRIVATE, 10, IPC_CREAT | 0600);
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    int *x = shmat(shmid, NULL, 0);
    *x = 1;

    for (int i = 0; i < nproc; i++) {
        if (!fork()) {
            while (1) {
                printf("%d %d\n", i + 1, *x);

                if (*x == xmax) {
                    semctl(semid, 0, IPC_RMID, 0);
                    shmctl(shmid, IPC_RMID, 0);
                    exit(0);
                }

                (*x)++;

                struct sembuf v = { (i == nproc - 1 ? 0 : i + 1), -1, 0 };

                if (semop(semid, &v, 1) < 0) {
                    exit(0);
                }

                struct sembuf p = { i, 1, 0 };
                semop(semid, &p, 1);
            }
        }
    }

    while (wait(NULL) != -1) {

    }

    return 0;
}