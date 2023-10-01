#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>

enum
{
    INT_SIZE = 4
};

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

int32_t
convert_from_be(unsigned char *arr)
{
    uint32_t n = arr[0] << 24 | arr[1] << 16 | arr[2] << 8 | arr[3];

    return n;
}

int
read_arr(int f, char *arr)
{
    for (int i = 0; i < INT_SIZE; i++) {
        int code;

        if ((code = read(f, &arr[i], sizeof(arr[i]))) != sizeof(arr[i])) {
            return code;
        }
    }

    return 1;
}

void
print_dec(int f, int idx)
{
    struct Node cur;
    lseek(f, sizeof(cur) * idx, SEEK_SET);

    unsigned char arr_key[INT_SIZE];
    unsigned char arr_left_idx[INT_SIZE];
    unsigned char arr_right_idx[INT_SIZE];

    if (read_arr(f, &arr_key[0]) != 1 || read_arr(f, &arr_left_idx[0]) != 1 || read_arr(f, &arr_right_idx[0]) != 1) {
        return;
    }

    cur.key = convert_from_be(arr_key);
    cur.left_idx = convert_from_be(arr_left_idx);
    cur.right_idx = convert_from_be(arr_right_idx);

    if (cur.right_idx) {
        print_dec(f, cur.right_idx);
    }

    printf("%d ", cur.key);

    if (cur.left_idx) {
        print_dec(f, cur.left_idx);
    }
}

int
main(int argc, char **argv)
{
    int f;

    if ((f = open(argv[1], O_RDONLY)) == -1) {
        return 1;
    }

    print_dec(f, 0);
    printf("\n");
    close(f);

    return 0;
}
