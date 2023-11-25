#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
    char *env = getenv("XDG_RUNTIME_DIR");

    if (!env) {
        env = getenv("TMPDIR");

        if (!env) {
            env = "/tmp";
        }
    }

    char name[PATH_MAX];
    snprintf(name, PATH_MAX, "%s/multscript-%d", env, getpid());

    char *const envp[] = {env, NULL};

    char buf[] = "#! /usr/bin/python3\n"
                 "import sys\n"
                 "import os\n"
                 "res = 1\n"
                 "for x in sys.argv[1:]:\n"
                 "  res *= int(x)\n"
                 "print(res)\n"
                 "os.remove(\'";

    int fd = open(name, O_CREAT | O_WRONLY | O_TRUNC, 0700);

    if (dprintf(fd, "%s%s\')\n", buf, name) < 0) {
        close(fd);
        unlink(name);

        exit(1);
    }
    
    close(fd);

    execve(name, argv, envp);
    unlink(name);
    exit(1);
}
