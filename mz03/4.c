
int
satsum(int v1, int v2)
{
    enum
    {
        MY_INT_MAX = (int) ((unsigned) ~0 >> !0),
        MY_INT_MIN = (int) ~((unsigned) ~0 >> !0)
    };

    if (v1 > 0 && v2 > MY_INT_MAX - v1) {
        return MY_INT_MAX;
    }

    if (v1 < 0 && v2 < MY_INT_MIN - v1) {
        return MY_INT_MIN;
    }

    return v1 + v2;
}
