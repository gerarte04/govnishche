#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
normalize_path(char *buf)
{
    char *str = buf;
    char *ptr = buf;

    while (buf[0] != '\0') {
        if (buf[0] == '/') {
            if (buf[1] == '.') {
                if (buf[2] == '.' && (buf[3] == '/' || buf[3] == '\0')) {
                    buf += 2;

                    if (ptr != str) {
                        do {
                            ptr--;
                        } while (ptr[0] != '/');
                    }

                    ptr--;
                } else if (buf[2] == '/' || buf[2] == '\0') {
                    buf++;
                    ptr--;
                }
            }
        }

        ptr++;
        buf++;
        ptr[0] = buf[0];
    }

    if (*(buf - 1) == '/') {
        *(ptr - 1) = '\0';
    } else {
        ptr[0] = '\0';
    }

    if (str[0] == '\0') {
        str[0] = '/';
        str[1] = '\0';
    }
}

void
ext_buf(char *buf, char *str, int slen, int *len, int *cap)
{
    int new_size = *len + slen + 1;

    while (new_size > *cap) {
        *cap *= 2;
    }

    buf = realloc(buf, *cap * sizeof(buf[0]));

    for (int i = 0; i < slen; i++) {
        buf[*len + i] = str[i];
    }

    buf[new_size - 1] = '\0';
    *len = new_size - 1;
}

char *
relativize_path(const char *path1, const char *path2)
{
    int size1 = strlen(path1) + 1;
    int size2 = strlen(path2) + 1;

    char path1_nrm[size1];
    char path2_nrm[size2];

    snprintf(path1_nrm, size1, "%s", path1);
    snprintf(path2_nrm, size2, "%s", path2);

    normalize_path(path1_nrm);
    normalize_path(path2_nrm);

    char *last_sl1 = path1_nrm;
    char *last_sl2 = path2_nrm;

    int i = 0;

    while (path1_nrm[i] == path2_nrm[i] && path1_nrm[i] != '\0' && path2_nrm[i] != '\0') {
        i++;

        if (path1_nrm[i] == '/') {
            last_sl1 = &path1_nrm[i];
            last_sl2 = &path2_nrm[i];
        }
    }

    if (last_sl2[0] == '\0') {
        last_sl2--;
    }

    int len = 0;
    int cap = 16;
    char *rel_path = malloc(cap * sizeof(rel_path[0]));
    rel_path[0] = '\0';

    int first = 1;

    for (last_sl1++; last_sl1[0] != '\0'; last_sl1++) {
        if (last_sl1[0] == '/') {
            if (first) {
                ext_buf(rel_path, "..", 2, &len, &cap);
                first = 0;
            } else {
                ext_buf(rel_path, "/..", 3, &len, &cap);
            }
        }
    }

    char *cat = ++last_sl2;
    int cat_len = 0;

    for (; last_sl2[0] != '\0'; last_sl2++) {
        if (last_sl2[0] != '/') {
            cat_len++;

            if (last_sl2[1] != '\0') {
                continue;
            }
        }

        if (first) {
            ext_buf(rel_path, cat, cat_len, &len, &cap);
            first = 0;
        } else {
            int buf_size = cat_len + 2;
            char buf[buf_size];
            snprintf(buf, buf_size, "/%s", cat);
            ext_buf(rel_path, buf, buf_size - 1, &len, &cap);
        }

        cat = last_sl2 + 1;
        cat_len = 0;
    }

    if (rel_path[0] == '\0') {
        rel_path = realloc(rel_path, 2 * sizeof(rel_path[0]));
        rel_path[0] = '.';
        rel_path[1] = '\0';
    }

    return rel_path;
}

int
main(int argc, char **argv)
{
    printf("%s\n", relativize_path("/a/b/c/d", "/a/e/f"));

    return 0;
}