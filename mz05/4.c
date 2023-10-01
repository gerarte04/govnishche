#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

enum
{
    START_CAP = 16
};

struct PathInfo
{
    char *name;
    struct stat st;
};

int
comp(const void *p1, const void *p2)
{
    int k = strcmp(((struct PathInfo *) p1)->name, ((struct PathInfo *) p2)->name);

    if (k < 0) {
        return -1;
    } else if (k > 0) {
        return 1;
    }

    return 0;
}

int
main(int argc, char **argv)
{
    int len = 0;
    int cap = START_CAP;
    struct PathInfo *paths = malloc(cap * sizeof(paths[0]));

    for (int i = 1; i < argc; i++) {
        struct stat arg_stat;

        if (stat(argv[i], &arg_stat) == 0) {
            int cont = 0;

            for (int j = 0; j < len; j++) {
                if (paths[j].st.st_ino == arg_stat.st_ino && paths[j].st.st_dev == arg_stat.st_dev) {
                    if (strcmp(paths[j].name, argv[i]) < 0) {
                        paths[j].name = argv[i];
                        paths[j].st = arg_stat;
                    }

                    cont = 1;
                    break;
                }
            }

            if (!cont) {
                struct PathInfo new_path;
                new_path.name = argv[i];
                new_path.st = arg_stat;

                if (len == cap) {
                    cap *= 2;
                    paths = realloc(paths, cap * sizeof(paths[0]));
                }

                paths[len] = new_path;
                len++;
            }
        }
    }

    qsort(paths, len, sizeof(paths[0]), comp);

    for (int i = 0; i < len; i++) {
        printf("%s\n", paths[i].name);
    }

    free(paths);

    return 0;
}
