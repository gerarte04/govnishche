#include <stdlib.h>
#include <string.h>

int
parse_rwx_permissions(const char *str)
{
    if (str == NULL) {
        return -1;
    }

    const char ex[] = "rwxrwxrwx";
    int len = strlen(str);
    unsigned int mode = 0;
    int i;

    for (i = 0; i < len; i++) {
        mode <<= 1;

        if (str[i] == ex[i]) {
            mode |= 1;
        } else if (str[i] != '-' || ex[i] == '\0') {
            return -1;
        }
    }

    if (ex[i] != '\0') {
        return -1;
    }

    return mode;
}
