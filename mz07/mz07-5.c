#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

enum
{
    YR_OFF = 1900,
    YR_MON = 8, // 256 день в году - 8 августа (если не високосный)
    YR_MDAY = 12,

    FM_MDAY = 26,
    FM_MON = 5,
    FM_YEAR = 2021,
    FM_HR = 11,
    FM_MIN = 14,

    PERIOD = ((29 * 24 + 12) * 60 + 44) * 60,

    CNT_MONDAY = 4,
    DAY_WEEK = 7
};

int
main(void)
{
    int y;
    scanf("%d", &y);

    struct tm tt;
    memset(&tt, 0, sizeof(tt));
    tt.tm_year = y - YR_OFF;
    tt.tm_mon = YR_MON;
    tt.tm_mday = YR_MDAY;

    if (y % 4 == 0) { // високосный год
        tt.tm_mday++;
    }

    time_t t = timegm(&tt);

    struct tm full_moon;
    memset(&full_moon, 0, sizeof(full_moon));
    full_moon.tm_year = FM_YEAR - YR_OFF;
    full_moon.tm_mon = FM_MON - 1;
    full_moon.tm_mday = FM_MDAY;
    full_moon.tm_hour = FM_HR;
    full_moon.tm_min = FM_MIN;
    time_t fm = timegm(&full_moon);

    long long diff = (long long) t - (long long) fm;
    time_t new_fm = t + (PERIOD * (diff > 0) - (diff) % PERIOD);

    gmtime_r(&new_fm, &full_moon);

    if (full_moon.tm_wday == 0) {
        full_moon.tm_mday -= DAY_WEEK;
    }

    full_moon.tm_mday += CNT_MONDAY * DAY_WEEK;
    timegm(&full_moon);

    if (full_moon.tm_wday == 0) {
        full_moon.tm_mday++;
    } else {
        full_moon.tm_mday -= full_moon.tm_wday - 1;
    }

    timegm(&full_moon);

    printf("%04d-%02d-%02d\n", full_moon.tm_year + YR_OFF, full_moon.tm_mon + 1, full_moon.tm_mday);

    return 0;
}
