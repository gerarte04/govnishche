#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    BACK_LEN = 3
};

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

char *
relativize_path(const char *path1, const char *path2)
{
    if (path1 == NULL || path2 == NULL) {
        return NULL;
    }

    int size1 = strlen(path1) + 1;
    int size2 = strlen(path2) + 1;

    char path1_nrm[size1];
    char path2_nrm[size2];

    snprintf(path1_nrm, size1, "%s", path1);
    snprintf(path2_nrm, size2, "%s", path2);

    normalize_path(path1_nrm);
    normalize_path(path2_nrm);

    int i = 0;
    int last_sl = 0;

    while (path1_nrm[i] == path2_nrm[i] && path1_nrm[i] != '\0' && path2_nrm[i] != '\0') {
        i++;

        if (path1_nrm[i] == '/') {
            last_sl = i;
        }
    }

    int sl_cnt = 0;

    for (i = last_sl + 1; path1_nrm[i] != '\0'; i++) {
        if (path1_nrm[i] == '/') {
            sl_cnt++;
        }
    }

    int len2 = (path2_nrm[last_sl] != '\0') ? strlen(&path2_nrm[last_sl + 1]) : 0;
    int rel_len = sl_cnt * BACK_LEN + len2;
    char *rel_path = calloc(rel_len + 1, sizeof(*rel_path));

    for (i = 0; i < BACK_LEN * sl_cnt; i += BACK_LEN) {
        snprintf(&rel_path[i], BACK_LEN + 1, "../");
    }

    if (len2 > 0) {
        snprintf(&rel_path[i], len2 + 1, "%s", &path2_nrm[last_sl + 1]);
    }

    if (rel_len > 0 && rel_path[rel_len - 1] == '/') {
        rel_path[rel_len - 1] = '\0';
    }

    if (rel_path[0] == '\0') {
        free(rel_path);
        rel_path = calloc(2, sizeof(*rel_path));
        rel_path[0] = '.';
        rel_path[1] = '\0';
    }

    return rel_path;
}
