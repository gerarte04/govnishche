#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

enum
{
    LET_CNT = 26,
    NAME_LEN = 50
};

char *
generate_name(char *name, int len)
{
    srand(time(NULL));
    
    for (int i = 0; i < len; i++) {
        name[i] = 'a' + rand() % LET_CNT;
    }

    return name;
}

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

    char cfile_name[PATH_MAX];
    char out_name[PATH_MAX];
    char buf[NAME_LEN];
    snprintf(cfile_name, PATH_MAX, "%s/%s%d.c", env, generate_name(buf, NAME_LEN), getpid());
    snprintf(out_name, PATH_MAX, "%s/%s%d", env, buf, getpid());

    char *const envp[] = {env, NULL};
    char *const new_argv[] = {out_name, cfile_name, NULL};

    char buf1[] = "#include <stdio.h>\n"
                  "#include <unistd.h>\n"
                  "int main(int argc, char **argv){\n"
                  "  char *reject = \"reject\";\n"
                  "  char *summon = \"summon\";\n"
                  "  char *disqualify = \"disqualify\";\n"
                  "  int x;\n"
                  "  while(scanf(\"%d\", &x) != EOF){\n"
                  "    char *ans = ";

    char buf2[] = ";\n"
                  "    printf(\"%s\\n\", ans);\n"
                  "  }\n"
                  "  unlink(argv[0]);\n"
                  "  unlink(argv[1]);\n"
                  "  return 0;\n"
                  "}\n";

    int fd = open(cfile_name, O_CREAT | O_WRONLY | O_TRUNC, 0600);

    if (dprintf(fd, "%s%s%s", buf1, argv[1], buf2) < 0) {
        close(fd);
        unlink(cfile_name);

        exit(1);
    }

    close(fd);

    if (!fork()) {
        execlp("gcc", "gcc", cfile_name, "-o", out_name, NULL);
    }

    wait(NULL);
    execve(out_name, new_argv, envp);

    unlink(cfile_name);
    unlink(out_name);
    exit(1);
}
