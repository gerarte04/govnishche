#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

// не тестил

int main(int argc, char **argv)
{
    int n = strtol(argv[1], NULL, 10);
    int lst = strtol(argv[2], NULL, 10);

    int semid = semget(IPC_PRIVATE, n + 1, IPC_CREAT | 0600);

    int shmid = shmget(IPC_PRIVATE, n * sizeof(int), IPC_CREAT | 0600);
    int *lifes = shmat(shmid, NULL, 0);

    for (int i = 0; i < n; i++) {
        if (!fork()) {
            lifes[i] = lst;
            
            while (lifes[i] > 0 && semop(semid, &(struct sembuf){i, -1, 0}, 1) >= 0) {
                lifes[i]--;
                printf("%d %u\n", i, lifes[i]);
                fflush(stdout);
                semop(semid, &(struct sembuf){n, 1, 0}, 1);
            }

            _exit(0);
        }
    }

    unsigned int nprc;

    while (scanf("%u", &nprc) != EOF) {
        if (lifes[nprc % n] != 0) {
            semop(semid, &(struct sembuf){nprc % n, 1, 0}, 1);
            semop(semid, &(struct sembuf){n, -1, 0}, 1);
        }
    }

    int cnt = 0;

    for (int i = 0; i < n; i++) {
        if (lifes[i] == 0) {
            cnt++;
        }
    }

    printf("%d\n", cnt);
    fflush(stdout);

    semctl(semid, 0, IPC_RMID, 0);
    shmctl(shmid, IPC_RMID, 0);

    while (wait(NULL) != -1) {
        
    }

    return 0;
}