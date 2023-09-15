#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 128

int main(void) {
    char s1[MAX_LEN];
    char s2[MAX_LEN];
    char s3[MAX_LEN];

    if (fgets(s1, MAX_LEN, stdin) == NULL) {
        printf("error: string 1 is not read\n");
        return 0;
    }
    
    if (fgets(s2, MAX_LEN, stdin) == NULL) {
        printf("error: string 2 is not read\n");
        return 0;
    }
    
    if (fgets(s3, MAX_LEN, stdin) == NULL) {
        printf("error: string 3 is not read\n");
        return 0;
    }

    s1[strlen(s1) - 1] = '\0';
    s2[strlen(s2) - 1] = '\0';
    s3[strlen(s3) - 1] = '\0';

    printf("[Host:%s,Login:%s,Password:%s]\n", s1, s2, s3);

    return 0;
}