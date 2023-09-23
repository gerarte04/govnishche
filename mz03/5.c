#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    MAX_BUF = 20
};

struct Elem
{
    struct Elem *next;
    char *str;
};

struct Elem *
dup_elem(struct Elem *head)
{
    if (!head) {
        return NULL;
    }

    struct Elem *new_head = head;
    struct Elem *elem = new_head;

    char *end = NULL;
    char *buf = calloc(MAX_BUF, sizeof(*buf));
    errno = 0;

    long n = strtol(head->str, &end, 10);

    if (!(*end || errno || end == head->str || (int) n != n)) {
        new_head = calloc(1, sizeof(*new_head));
        snprintf(buf, MAX_BUF, "%d", (int) (n + 1));
        new_head->str = strdup(buf);

        new_head->next = head;
        elem = head;
    }

    while (elem->next) {
        end = NULL;
        errno = 0;
        n = strtol(elem->next->str, &end, 10);

        if (!(*end || errno || end == elem->next->str || (int) n != n || (int) (n + 1) < n + 1)) {
            struct Elem *new_elem = calloc(1, sizeof(*new_elem));
            snprintf(buf, MAX_BUF, "%d", (int) (n + 1));
            new_elem->str = strdup(buf);

            new_elem->next = elem->next;
            elem->next = new_elem;
            elem = elem->next;
        }

        elem = elem->next;
    }

    free(buf);

    return new_head;
}
