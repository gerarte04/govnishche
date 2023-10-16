#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

enum
{
    YR_OFF = 1900,
    YR_YDAY = 256,

    FM_MDAY = 26,
    FM_MON = 5,
    FM_YEAR = 2021,
    FM_HR = 11,
    FM_MIN = 14,

    M_DAYOFF = 29,
    M_HROFF = 12,
    M_MINOFF = 44,

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
    tt.tm_mday = 13;
    tt.tm_mon = 8;
    tt.tm_isdst = -1;
    mktime(&tt);

    struct tm full_moon;
    memset(&full_moon, 0, sizeof(full_moon));
    full_moon.tm_year = FM_YEAR - YR_OFF;
    full_moon.tm_mon = FM_MON - 1;
    full_moon.tm_mday = FM_MDAY;
    full_moon.tm_hour = FM_HR;
    full_moon.tm_min = FM_MIN;
    full_moon.tm_isdst = -1;
    mktime(&full_moon);

    if (full_moon.tm_year <= tt.tm_year) {
        while (full_moon.tm_year < tt.tm_year || full_moon.tm_yday <= tt.tm_yday) {
            full_moon.tm_mday += M_DAYOFF;
            full_moon.tm_hour += M_HROFF;
            full_moon.tm_min += M_MINOFF;
            mktime(&full_moon);
        }
    } else {
        struct tm new_fm = full_moon;

        while (new_fm.tm_year > tt.tm_year || new_fm.tm_yday > tt.tm_yday) {
            full_moon = new_fm;
            new_fm.tm_mday -= M_DAYOFF;
            new_fm.tm_hour -= M_HROFF;
            new_fm.tm_min -= M_MINOFF;
            mktime(&new_fm);
        }
    }

    if (full_moon.tm_wday == 0) {
        full_moon.tm_mday -= DAY_WEEK;
    }

    full_moon.tm_mday += CNT_MONDAY * DAY_WEEK;
    mktime(&full_moon);

    if (full_moon.tm_wday == 0) {
        full_moon.tm_mday++;
    } else {
        full_moon.tm_mday -= full_moon.tm_wday - 1;
    }

    printf("%04d-%02d-%02d\n", full_moon.tm_year + YR_OFF, full_moon.tm_mon + 1, full_moon.tm_mday);

    return 0;
}
