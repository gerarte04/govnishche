#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>

enum
{
    BASE = 10
};

struct msg
{
    long type;
    long long x1;
    long long x2;
};

int
main(int argc, char **argv)
{
    key_t key = strtol(argv[1], NULL, BASE);
    int n = strtol(argv[2], NULL, BASE);
    int value1 = strtoll(argv[3], NULL, BASE);
    int value2 = strtoll(argv[4], NULL, BASE);
    int maxval = strtoll(argv[5], NULL, BASE);

    int msqid = msgget(key, IPC_CREAT | 0600);
    int msgsz = sizeof(struct msg) - sizeof(long);
    msgsnd(msqid, &(struct msg) {1, value1, value2}, msgsz, 0);

    for (int i = 0; i < n; i++) {
        if (!fork()) {
            struct msg m;

            while (msgrcv(msqid, &m, msgsz, i + 1, 0) >= 0) {
                long long x3 = m.x1 + m.x2;
                printf("%d %lld\n", i, x3);
                fflush(stdout);

                if (x3 > maxval) {
                    msgctl(msqid, IPC_RMID, NULL);
                    _exit(0);
                }

                msgsnd(msqid, &(struct msg) {x3 % n + 1, m.x2, x3}, msgsz, 0);
            }

            _exit(0);
        }
    }

    while (wait(NULL) != -1) {
    }

    return 0;
}
