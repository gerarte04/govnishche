enum
{
    MY_INT_MAX = 2147483647,
    MY_INT_MIN = -2147483648
};

int
satsum(int v1, int v2)
{
    int r = v1 + v2;
    int temp;

    if (__builtin_add_overflow(v1, v2, &temp)) {
        if (v1 > 0) {
            r = MY_INT_MAX;
        } else {
            r = MY_INT_MIN;
        }
    }

    return r;
}
