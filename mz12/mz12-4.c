#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
    char name[] = "script";

    char *env = getenv("XDG_RUNTIME_DIR");

    if (!env) {
        env = getenv("TMPDIR");

        if (!env) {
            env = "/tmp";
        }
    }

    char *const envp[] = {env, NULL};

    char buf[] = "#! /usr/bin/python3\n"
                 "import sys\n"
                 "res = 1\n"
                 "for x in sys.argv[1:]:\n"
                 "  res *= int(x)\n\n"
                 "print(res)\n";

    int fd = open(name, O_CREAT | O_WRONLY | O_TRUNC, 0700);
    dprintf(fd, "%s", buf);
    close(fd);

    if (!fork()) {
        execve(name, argv, envp);
        unlink(name);
        exit(1);
    }

    wait(NULL);
    unlink(name);

    return 0;
}
