STYPE
bit_reverse(STYPE value)
{
    UTYPE a = (UTYPE) value;
    UTYPE res = 0;

    for (int i = 0; i < sizeof(value) * 8; i++) {
        res <<= 1;
        res += a & 1;
        a >>= 1;
    }

    return (STYPE) res;
}
