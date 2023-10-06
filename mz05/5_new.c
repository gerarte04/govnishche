#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *
relativize_path(const char *path1, const char *path2)
{
    const char *last_sl1 = path1;
    const char *last_sl2 = path2;

    while (*path1 == *path2 && *path1 != '\0' && *path2 != '\0') {
        path1++;
        path2++;

        if (*path1 == '/') {
            last_sl1 = path1;
            last_sl2 = path2;
        }
    }

    if (*last_sl2 == '\0') {
        last_sl2--;
    }

    int cap;
    char *rel_path;

    const char *finder = last_sl1;
    int has_sl = 0;

    while (*(++finder) != '\0') {
        if (*finder == '/') {
            has_sl = 1;
            break;
        }
    }

    if (!has_sl) {
        cap = 2;
        rel_path = malloc(cap * sizeof(rel_path[0]));
        
        rel_path[0] = '.';
        rel_path[1] = '\0';
    } else {
        cap = 1;
        rel_path = malloc(cap * sizeof(rel_path[0]));
        
        rel_path[0] = '\0';
    }

    int first = 1;

    for (last_sl1++; *last_sl1 != '\0'; last_sl1++) {
        if (*last_sl1 == '/') {
            int new_cap;
            
            if (first) {
                new_cap = cap + 2;
                rel_path = realloc(rel_path, new_cap * sizeof(rel_path[0]));

                rel_path[cap - 1] = '.';
                rel_path[cap] = '.';
                rel_path[cap + 1] = '\0';

                first = 0;
            } else {
                new_cap = cap + 3;
                rel_path = realloc(rel_path, new_cap * sizeof(rel_path[0]));

                rel_path[cap - 1] = '/';
                rel_path[cap] = rel_path[cap + 1] = '.';
                rel_path[cap + 2] = '\0';
            }

            cap = new_cap;
        }
    }

    char buf[PATH_MAX + 1];
    int len = 0;

    for (last_sl2++; *last_sl2 != '\0'; last_sl2++) {
        if (*last_sl2 != '/') {
            buf[len] = *last_sl2;
            len++;

            if (*(last_sl2 + 1) != '\0') {
                continue;
            }
        }
        
        int new_cap = cap + len + 1;
        rel_path = realloc(rel_path, new_cap * sizeof(rel_path[0]));

        rel_path[cap - 1] = '/';
        rel_path[cap + len] = '\0';

        for (int i = 0; i < len; i++) {
            rel_path[cap + i] = buf[i];
        }

        cap = new_cap;
        len = 0;
    }

    return rel_path;
}

int
main(int argc, char **argv)
{
    printf("%s\n", relativize_path("/snap/bin/spotify.exe", "/home/kingmidas/Desktop"));

    return 0;
}