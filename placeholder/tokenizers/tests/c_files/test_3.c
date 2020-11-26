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
