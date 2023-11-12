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
