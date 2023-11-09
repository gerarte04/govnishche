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

    CTABLE_SIZE = 64,
    UTABLE_SIZE = 123,

    SPACE = 65,
    ERR = 66
};

const char conv_char_table[CTABLE_SIZE] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
    'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
    's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_'};

const uint8_t conv_uint8_table[UTABLE_SIZE] = {
    ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR,
    ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR,
    ERR, ERR, ERR, 62,  ERR, ERR, 52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  ERR, ERR, ERR, ERR, ERR,
    ERR, ERR, 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,  16,  17,  18,
    19,  20,  21,  22,  23,  24,  25,  ERR, ERR, ERR, ERR, 63,  ERR, 26,  27,  28,  29,  30,  31,  32,  33,
    34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51};

char
convert_uint8(uint8_t n)
{
    if (n >= 0 && n < CTABLE_SIZE) {
        return conv_char_table[n];
    }

    return '\0';
}

uint8_t
convert_char(char c)
{
    if (isspace(c)) {
        return SPACE;
    } else if (c >= 0 && c < UTABLE_SIZE) {
        return conv_uint8_table[(uint8_t) c];
    }

    return ERR;
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
                if (stk[cnt_strd] == SPACE) {
                    i++;
                    str++;
                } else if (stk[cnt_strd] == ERR) {
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
