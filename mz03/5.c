#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

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

    char *buf = NULL;
    errno = 0;

    long n = strtol(head->str, &buf, 10);

    if (!(*buf || errno || buf == head->str || (int) n != n)) {
        new_head = malloc(sizeof(struct Elem));
        new_head->str = malloc(MAX_BUF);
        snprintf(new_head->str, MAX_BUF, "%d", (int) (n + 1));
        new_head->next = head;
        elem = head;
    }

    while (elem->next) {
        buf = NULL;
        errno = 0;
        n = strtol(elem->next->str, &buf, 10);

        if (!(*buf || errno || buf == elem->next->str || (int) n != n || (int) (n + 1) < n + 1)) {
            errno = 0;
            struct Elem *new_elem = malloc(sizeof(struct Elem));
            new_elem->str = malloc(MAX_BUF);
            snprintf(new_elem->str, MAX_BUF, "%d", (int) (n + 1));
            new_elem->next = elem->next;
            elem->next = new_elem;
            elem = elem->next;
        }

        elem = elem->next;
    }

    return new_head;
}
