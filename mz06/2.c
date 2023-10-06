void
normalize_path(char *buf)
{
    char *str = buf;

    while (*buf != '\0') {
        if (*buf == '/') {
            if (*(buf + 1) == '\0' && buf != str) {
                *buf = '\0';
            }

            if (*(buf + 1) == '.') {
                int go_cur = *(buf + 2) == '/' || *(buf + 2) == '\0';
                int go_back = *(buf + 2) == '.' && (*(buf + 3) == '/' || *(buf + 3) == '\0');

                if (go_cur || go_back) {
                    char *src = (go_cur) ? buf + 2 : buf + 3;
                    char *dst = buf;

                    if (go_back && buf != str) {
                        do {
                            dst--;
                        } while (*dst != '/');
                    }

                    buf = dst - 1;

                    if (*src == '\0') {
                        *(dst + 1) = '\0';
                    } else {
                        while (*(src - 1) != '\0') {
                            *dst = *src;
                            dst++;
                            src++;
                        }
                    }
                }
            }
        }

        buf++;
    }
}
