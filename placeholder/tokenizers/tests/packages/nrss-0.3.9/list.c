/* NRSS - ncurses RSS reader
   Copyright (C) 2007 Jack Miller <jjm2n4@umr.edu>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as 
   published by the Free Software Foundation.
*/
#include "nrss.h"

void ladd_item(struct item **head, struct item *newitem)
{
    struct item *cur = NULL;

    if (*head == NULL) {
        *head = newitem;
        (*head)->next = NULL;
        (*head)->prev = newitem;
    } else if (!nw((*head))) {
        newitem->next = *head;
        newitem->prev = (*head)->prev;
        (*head)->prev = newitem;
        *head = newitem;
    } else {
        cur = *head;

        while ((cur->next) && (nw(cur->next)))
            cur = cur->next;

        if (!cur->next)
            (*head)->prev = newitem;
        else
            cur->next->prev = newitem;

        newitem->next = cur->next;
        newitem->prev = cur;
        cur->next = newitem;
    }
}

int lsearch(struct item *items, char *title)
{
    if(!title)
        return 0;

    struct item *cur = NULL;

    if(!title)
        return 1;

    for (cur = items; cur; cur = cur->next) {
        if (!strcmp(title, cur->title))
            return 0;
    }

    return 1;
}

void lfree_item(struct item *head)
{
    if (head) {
        lfree_item(head->next);
        free(head->title);
        free(head->cached_line);
        free(head->link);
        free(head);
    }
}

void ladd_feed(struct feed **head, struct feed *newfeed)
{
    struct feed *cur = NULL;

    if (*head == NULL) {
        *head = newfeed;
        (*head)->next = NULL;
        (*head)->prev = newfeed;
    } else {
        cur = (*head)->prev;
        (*head)->prev = newfeed;
        cur->next = newfeed;
        newfeed->prev = cur;
        newfeed->next = NULL;
    }
}

void lfree_feed(struct feed *head)
{
    if (head) {
        lfree_feed(head->next);
        lfree_item(head->items);
        free(head->title);
        free(head->cached_line);
        free(head->link);
        free(head);
    }
}
