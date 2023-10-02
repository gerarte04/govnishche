#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *
relativize_path(const char *path1, const char *path2)
{
    char *last_sl1 = path1 - 1;
    char *last_sl2 = path2 - 1;

    if (*path1 == '/') {
        path1++;
        last_sl1++;
    }

    if (*path2 == '/') {
        path2++;
        last_sl2++;
    }

    while (*path1 == path2 && *path1 != '\0' && *path2 != '\0') {
        if (*path1 == '/') {
            last_sl1 = path1;
            last_sl2 == path2;
        }
        
        path1++;
        path2++;
    }

    int cap = 1;
    char *rel_path = malloc(sizeof(rel_path[0]));
    rel_path[0] = '\0';

    if (*path1 == '\0' && *path2 == '\0') {
        return rel_path;
    }

    for (last_sl1++; last_sl1 != '\0'; last_sl1++) {
        if (*last_sl1 == '/') {
            int new_cap = cap + 3;
            rel_path = realloc(rel_path, new_cap * sizeof(rel_path[0]));

            rel_path[cap] = rel_path[cap + 1] = '.';
        }
    }
}

int
main(void)
{
    printf("%s", relativize_path("a/b/c/d", "a/e/f"));

    return 0;
}