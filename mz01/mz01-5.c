#include <stdio.h>

enum
{
    MIN_N = 1,
    MAX_N = 9
};

void
print_arr(int *arr, int k)
{
    for (int i = 0; i < k; i++) {
        printf("%d", arr[i] + 1);
    }

    printf("\n");
}

int
main(void)
{
    int n;

    if (scanf("%d", &n) != 1) {
        printf("input error\n");
        return 1;
    }

    if (n < MIN_N || n > MAX_N) {
        printf("incorrect input value\n");
        return 1;
    }

    int arr[MAX_N];
    int check[MAX_N];

    for (int i = 0; i < n; i++) {
        arr[i] = i;
        check[i] = 1;
    }

    check[n - 1] = 0;
    int i = n - 2;

    print_arr(&arr[0], n);

    while (i >= 0) {
        check[arr[i]] = 0;

        if (arr[i] < arr[i + 1]) {
            do {
                arr[i]++;
            } while (check[arr[i]]);

            check[arr[i]] = 1;

            int j = 0;

            for (i++; i < n; i++) {
                for (; check[j]; j++) {
                }

                check[j] = 1;
                arr[i] = j;
            }

            check[arr[n - 1]] = 0;
            i = n - 1;

            print_arr(&arr[0], n);
        }

        i--;
    }

    return 0;
}
