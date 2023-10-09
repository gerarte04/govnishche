#include <stdlib.h>

struct s1
{
    char f1;
    long long f2;
    char f3;
};

struct s2
{
    long long f2;
    char f1;
    char f3;
};

size_t
compactify(void *ptr, size_t size)
{
    struct s1 *s1_ptr = (struct s1 *) ptr;
    struct s1 *arrend = s1_ptr + size / sizeof(*s1_ptr);
    struct s2 *s2_ptr = (struct s2 *) ptr;
    size_t size2 = 0;

    for (; s1_ptr < arrend; s1_ptr++) {
        struct s2 s2_val;
        s2_val.f1 = s1_ptr->f1;
        s2_val.f2 = s1_ptr->f2;
        s2_val.f3 = s1_ptr->f3;

        *s2_ptr = s2_val;
        s2_ptr++;
        size2 += sizeof(*s2_ptr);
    }

    return size2;
}
