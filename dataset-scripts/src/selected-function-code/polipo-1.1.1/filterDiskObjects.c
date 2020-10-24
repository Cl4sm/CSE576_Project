DiskObjectPtr
filterDiskObjects(DiskObjectPtr from, const char *root, int recursive)
{
    int n = strlen(root);
    DiskObjectPtr p, q;

    while(from && !filter(from, root, n, recursive)) {
        p = from;
        from = p->next;
        free(p->location);
        free(p);
    }

    p = from;
    while(p && p->next) {
        if(!filter(p->next, root, n, recursive)) {
            q = p->next;
            p->next = q->next;
            free(q->location);
            free(q);
        } else {
            p = p->next;
        }
    }
    return from;
}