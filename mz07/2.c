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
    TH_OFF = 2,
    DAY_DIV = 3

    // YR_OFF - смещение года для хранения в struct tm
    // MON_CNT - количество месяцев в году
    // THURS - номер четверга в неделе
    // DAY_WEEK - количество дней в неделе
    // TH_OFF - коэфф-т смещения в неделях между вторым и четвертым четвергом
    // DAY_DIV - делитель для проверки делимости номера дня в месяце
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

        if (tt.tm_mday % DAY_DIV != 0) {
            printf("%d %d\n", i + 1, tt.tm_mday);
        }

        tt.tm_mday += TH_OFF * DAY_WEEK;
        mktime(&tt);

        if (tt.tm_mday % DAY_DIV != 0) {
            printf("%d %d\n", i + 1, tt.tm_mday);
        }
    }

    return 0;
}
