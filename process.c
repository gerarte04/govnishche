#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;

    if ((pid = fork()) > 0) {
        printf("ls completed successfully\n");
    } else {
        printf("PID: %d\n", getpid());
        execl("/bin/ls", "ls", "-l", "-a", NULL);
    }

    return 0;
}