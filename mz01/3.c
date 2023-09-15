#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;
    scanf("%d", &n);

    int **arr = calloc(n, sizeof(int *));

    for (int i = 0; i < n; i++) {
        arr[i] = calloc(n, sizeof(int));
        arr[i][1] = i;
    }

    for (int i = 2; i < n; i++) {
        for (int j = 0; j < n; j++) {
            arr[(j + arr[j][i - 1]) % n][i] = arr[j][i - 1];
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 1; j < n; j++) {
            printf("%d ", arr[i][j]);
        }

        printf("\n");
        free(arr[i]);
    }

    free(arr);

    return 0;
}