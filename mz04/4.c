#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>

enum
{
    STRUCT_SIZE = 12,
    CHECK = 0x00000011,
    LIL_END = 17
};

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

int32_t
read_from_be(unsigned char *arr)
{
    uint32_t check = CHECK;

    if (check == LIL_END) {
        return arr[0] << 24 | arr[1] << 16 | arr[2] << 8 | arr[3];
    } else {
        return arr[0] | arr[1] << 8 | arr[2] << 16 | arr[3] << 24;
    }
}

void
print_dec(int f, int idx)
{
    struct Node cur;
    lseek(f, sizeof(cur) * idx, SEEK_SET);

    unsigned char arr[STRUCT_SIZE];
    int cnt = sizeof(arr[0]) * STRUCT_SIZE;

    if (read(f, arr, cnt) != cnt) {
        close(f);
        exit(1);
    }

    cur.key = read_from_be(&arr[0]);
    cur.left_idx = read_from_be(&arr[sizeof(cur.key)]);
    cur.right_idx = read_from_be(&arr[sizeof(cur.key) + sizeof(cur.left_idx)]);

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
    int f = open(argv[1], O_RDONLY);
    print_dec(f, 0);
    printf("\n");
    close(f);

    return 0;
}
