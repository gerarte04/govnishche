#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/stat.h>

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

void print_dec(int f, int idx)
{
    struct Node *cur = calloc(1, sizeof(*cur));

    lseek(f, sizeof(*cur) * idx, 0);
    read(f, cur, sizeof(*cur));

    if (cur->right_idx) {
        print_dec(f, cur->right_idx);
    }
    
    printf("%d ", cur->key);

    if (cur->left_idx) {
        print_dec(f, cur->left_idx);
    }

    free(cur);
}

int
main(int argc, char **argv)
{
    int f = open(argv[1], O_RDONLY, 0777);
    print_dec(f, 0);
    printf("\n");
    close(f);

    return 0;
}