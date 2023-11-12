STYPE
bit_reverse(STYPE value)
{
    UTYPE a = (UTYPE) value;
    UTYPE res = 0;
    UTYPE temp = ~0;

    while (temp) {
        res <<= 1;
        res += a & 1;
        a >>= 1;
        temp >>= 1;
    }

    return (STYPE) res;
}
