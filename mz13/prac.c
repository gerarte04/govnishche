#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

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

int main(int argc, char **argv)
{
    int nproc = strtol(argv[1], NULL, 10);
    int xmax = strtol(argv[2], NULL, 10);

    int semid = semget(IPC_PRIVATE, 10, IPC_CREAT | 0600);
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    
    int *x = shmat(shmid, NULL, 0);
    *x = 1;

    struct sembuf psems[nproc];
    struct sembuf vsems[nproc];

    for (int i = 0; i < nproc; i++) {
        psems[i] = (struct sembuf) { i, -1, 0 };
        vsems[i] = (struct sembuf) { i, 1, 0 };
    }

    semop(semid, &vsems[0], 1);

    for (int i = 0; i < nproc; i++) {
        if (!fork()) {
            while (semop(semid, &psems[i], 1) >= 0) {
                printf("%d %d\n", i + 1, *x);

                if (*x == xmax) {
                    printf("done\n");
                    semctl(semid, 0, IPC_RMID, 0);
                    shmctl(shmid, IPC_RMID, 0);
                    exit(0);
                }

                (*x)++;

                semop(semid, &vsems[i == nproc - 1 ? 0 : i + 1], 1);
            }

            exit(0);
        }
    }

    while (wait(NULL) != -1) {
    }

    return 0;
}