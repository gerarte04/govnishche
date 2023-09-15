#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PATH_MAX 2048
#define A_CODE 97

void my_strcpy(char *s1, const char *s2) {
    while (*s2) {
        *s1 = *s2;
        s2++;
        s1++;
    }

    *s1 = '\0';
}

int my_strcmp(const char *s1, const char *s2) {
    while ((*s1 == *s2) && *s1++ && *s2++);

    return *s1 - *s2;
}

int main(int argc, char **argv) {
    int cnts[26] = { };

    char path[PATH_MAX];
    fgets(path, PATH_MAX, stdin);

    FILE* f = fopen(path, "r");
    int c;

    while ((c = fgetc(f)) != EOF) {
        if (isalpha(c)) {
            ++cnts[tolower(c) - 97];
        }
    }

    fclose(f);

    for (int i = 0; i < 26; i++) {
        printf("\'%c\' - %d\n", i + 97, cnts[i]);
    }

    return 0;
}