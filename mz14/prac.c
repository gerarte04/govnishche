#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <unistd.h>

struct msg
{
    long type;
    int n;
};

int f1(int argc, char **argv)
{
    int n = strtol(argv[1], NULL, 10);
    int xmax = strtol(argv[2], NULL, 10);
    int msgid = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
    msgsnd(msgid, &(struct msg){1, 1}, sizeof(int), 0);

    for (int i = 1; i <= n; i++) {
        if (!fork()) {
            struct msg m;
            while (msgrcv(msgid, &m, sizeof(int), i, 0) >= 0) {
                printf("%d %d\n", i, m.n);

                if (m.n == xmax) {
                    msgctl(msgid, IPC_RMID, 0);
                    exit(0);
                }

                m.n++;
                m.type = i % n + 1;
                msgsnd(msgid, &m, sizeof(int), 0);
            }

            exit(0);
        }
    }

    while (wait(NULL) != -1) {
    }

    return 0;
}

struct seg
{
    int size;
    int base;
};

int get_physaddr(struct seg *table, int vaddr)
{
    int nseg = (unsigned int) vaddr >> 29;
    int off = (unsigned int) vaddr & 0x1FFFFFFF;

    if (off > table[nseg].size) {
        return 25;
    }

    return table[nseg].base + off;
}

const char *disks[4] = { "DISK0", "DISK1", "DISK2", "DISK3" };
const int bsize;

void read_block(int num, char *buf)
{
    int ndisk = num % 4;
    int off = num / 4;

    int fd = open(disks[ndisk], O_RDONLY);
    lseek(fd, off * bsize, SEEK_SET);
    read(fd, buf, bsize);
    close(fd);
}

int get_size(int *table, int start)
{
    int num = start;
    int size = 0;

    while (num != 0) {
        num = table[num];
        size++;
    }

    return size;
}

struct row
{
    int pid;
    int nvirtp;
};

int getphys_invert(struct row *table, int size, int vaddr)
{
    unsigned off = vaddr & 0xFFF;
    unsigned vpage = vaddr >> 12;

    for (int i = 0; i < size; i++) {
        if (table[i].pid == getpid() && table[i].nvirtp == vpage) {
            return i << 12 | off;
        }
    }

    return NULL;
}

int main(int argc, char **argv)
{
    
}
