#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

enum
{
    YR_OFF = 1900,
    MON_CNT = 12,
    THURS = 4,
    DAY_WEEK = 7,
    COEF2 = 2
};

int
main(void)
{
    int y;
    if (scanf("%d", &y) != 1) {
        return 1;
    }

    struct tm tt;

    for (int i = 0; i < MON_CNT; i++) {
        memset(&tt, 0, sizeof(tt));
        tt.tm_year = y - YR_OFF;
        tt.tm_isdst = -1;
        tt.tm_mday = 1;
        tt.tm_mon = i;
        mktime(&tt);

        if (tt.tm_wday <= THURS) {
            tt.tm_mday += THURS - tt.tm_wday;
        } else {
            tt.tm_mday += DAY_WEEK - (tt.tm_wday - THURS);
        }

        tt.tm_mday += DAY_WEEK;
        mktime(&tt);

        if (tt.tm_mday % 3 != 0) {
            printf("%d %d\n", i + 1, tt.tm_mday);
        }

        tt.tm_mday += COEF2 * DAY_WEEK;
        mktime(&tt);

        if (tt.tm_mday % 3 != 0) {
            printf("%d %d\n", i + 1, tt.tm_mday);
        }
    }

    return 0;
}
