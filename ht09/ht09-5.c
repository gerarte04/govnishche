#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

enum
{
    SIGNIF_LEN = 7,
    MASK_HIGH = 0x7F,
    HIGH_BIT = 1u << 7,
    MAX_SHIFT = 57
};

struct ReadContext
{
    uint8_t *buf;
    size_t size;
    size_t pos;
    uint64_t value_u64;
};

size_t
uleb128_len(uint64_t value)
{
    size_t len = 1;

    while (value >>= SIGNIF_LEN) {
        len++;
    }

    return len;
}

uint8_t *
uleb128_encode(uint8_t *ptr, uint64_t value)
{
    do {
        *ptr = value & MASK_HIGH;

        if (value >>= SIGNIF_LEN) {
            *ptr |= HIGH_BIT;
        }

        ptr++;
    } while (value);

    return ptr;
}

int
uleb128_decode(struct ReadContext *cntx)
{
    if (cntx->pos >= cntx->size || cntx->size <= 0) {
        return 0;
    }

    uint8_t *buf = cntx->buf + cntx->pos;
    size_t size = cntx->size - cntx->pos;
    uint64_t num = 0;
    size_t len = 0;

    int shift = 0;
    int end;

    do {
        uint8_t signif = *buf & MASK_HIGH;

        if ((signif == 0 && len != 0) || shift > MAX_SHIFT) {
            return 0;
        }

        num |= signif << shift;
        shift += SIGNIF_LEN;
        len++;
    } while ((end = *buf++ >> SIGNIF_LEN) && len < size);

    if (end) {
        return 0;
    }

    cntx->value_u64 = num;
    cntx->pos += len;

    return 1;
}

int main(void)
{
    uint8_t arr[1] = { 1 };

    struct ReadContext rc = { arr, 2, 1, 0 };

    printf("%d\n", uleb128_decode(&rc));
    printf("%ld\n", rc.value_u64);

    printf("%d\n", uleb128_decode(&rc));
    printf("%ld\n", rc.value_u64);

    return 0;
}