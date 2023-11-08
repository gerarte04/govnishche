#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

enum
{
    BASE64_BIT = 6,
    ESTK_SIZE = 3,
    DSTK_SIZE = 4,

    OFF1 = 2,
    OFF2 = 4,
    OFF3 = 6,

    MASK1 = 0x03,
    MASK2 = 0x0F,
    MASK3 = 0x3F,

    HYPH_CODE = 62,
    UNDERL_CODE = 63,
    UPPER_START = 0,
    LOWER_START = UPPER_START + ('Z' - 'A' + 1),
    NUM_START = LOWER_START + ('z' - 'a' + 1),
    NUM_END = HYPH_CODE - 1,
    LOWER_END = NUM_START - 1,
    UPPER_END = LOWER_START - 1,
    OFFSET_NUM = '0' - NUM_START,
    OFFSET_LOWER = 'a' - LOWER_START,
    OFFSET_UPPER = 'A' - UPPER_START,

    SPACE_CODE = 65,
    ERR_CODE = 66
};

char
convert_uint8(uint8_t n)
{
    if (n == HYPH_CODE) {
        return '-';
    } else if (n == UNDERL_CODE) {
        return '_';
    } else if (n >= NUM_START && n <= NUM_END) {
        return n + OFFSET_NUM;
    } else if (n >= LOWER_START && n <= LOWER_END) {
        return n + OFFSET_LOWER;
    } else if (n >= UPPER_START && n <= UPPER_END) {
        return n + OFFSET_UPPER;
    }

    return '\0';
}

uint8_t
convert_char(char c)
{
    if (c == '-') {
        return HYPH_CODE;
    } else if (c == '_') {
        return UNDERL_CODE;
    } else if (c >= '0' && c <= '9') {
        return c - OFFSET_NUM;
    } else if (c >= 'a' && c <= 'z') {
        return c - OFFSET_LOWER;
    } else if (c >= 'A' && c <= 'Z') {
        return c - OFFSET_UPPER;
    } else if (isspace(c)) {
        return SPACE_CODE;
    }

    return ERR_CODE;
}

char *
b64u_encode(const uint8_t *data, size_t size)
{
    int new_size = size * CHAR_BIT;
    new_size = (new_size % BASE64_BIT) ? new_size / BASE64_BIT + 1 : new_size / BASE64_BIT;

    char *code;

    if ((code = calloc(new_size + 1, sizeof(*code))) == NULL) {
        return NULL;
    }

    uint8_t stk[ESTK_SIZE];
    int ind = 0;
    int cnt_strd;

    for (int i = 0; i < size; i += ESTK_SIZE) {
        cnt_strd = 0;

        for (int j = 0; j < ESTK_SIZE; j++) {
            if (i < size - j) {
                stk[j] = data[j];
                cnt_strd++;
            } else {
                stk[j] = 0;
                break;
            }
        }

        code[ind++] = convert_uint8(stk[0] >> OFF1);
        code[ind++] = convert_uint8(stk[1] >> OFF2 | (stk[0] & MASK1) << OFF2);

        if (cnt_strd >= ESTK_SIZE - 1) {
            code[ind++] = convert_uint8(stk[2] >> OFF3 | (stk[1] & MASK2) << OFF1);
        }

        if (cnt_strd == ESTK_SIZE) {
            code[ind++] = convert_uint8(stk[2] & MASK3);
        }

        data += ESTK_SIZE;
    }

    code[new_size] = '\0';

    return code;
}

int
b64u_decode(const char *str, uint8_t **p_data, size_t *p_size)
{
    int len = strlen(str);
    int new_size = len * BASE64_BIT / CHAR_BIT;
    uint8_t *data = calloc(new_size + 1, sizeof(*data));

    if (data == NULL) {
        *p_data = NULL;
        return 1;
    }

    uint8_t stk[DSTK_SIZE];
    int ind = 0;
    int cnt_strd;
    int i;

    for (i = 0; i < len; i += DSTK_SIZE) {
        cnt_strd = 0;

        while (cnt_strd < DSTK_SIZE && i < len) {
            if (i < len - cnt_strd) {
                stk[cnt_strd] = convert_char(str[cnt_strd]);
                if (stk[cnt_strd] == SPACE_CODE) {
                    i++;
                    str++;
                } else if (stk[cnt_strd] == ERR_CODE) {
                    free(data);
                    return 0;
                } else {
                    cnt_strd++;
                }
            } else {
                stk[cnt_strd] = 0;
                break;
            }
        }

        if (cnt_strd >= 2) {
            data[ind++] = stk[0] << OFF1 | (stk[1] >> OFF2 & MASK1);
        }

        if (cnt_strd >= 3) {
            data[ind++] = ((stk[1] & MASK2) << OFF2) | (stk[2] >> OFF1 & MASK2);
        }

        if (cnt_strd == DSTK_SIZE) {
            data[ind++] = ((stk[2] & MASK1) << OFF3) | (stk[3] & MASK3);
        }

        str += DSTK_SIZE;
    }

    data[new_size] = 0;
    *p_data = data;
    *p_size = new_size;

    return 1;
}
