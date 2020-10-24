DiskObjectPtr
insertDirs(DiskObjectPtr from)
{
    DiskObjectPtr p, q, new;
    int n, m;
    char *cp;

    p = NULL; q = from;
    while(q) {
        n = strlen(q->location);
        if(n > 0 && q->location[n - 1] != '/') {
            cp = strrchr(q->location, '/');
            m = cp - q->location + 1;
            if(cp && (!p || strlen(p->location) < m ||
                      memcmp(p->location, q->location, m) != 0)) {
                new = malloc(sizeof(DiskObjectRec));
                if(!new) break;
                new->location = strdup_n(q->location, m);
                if(new->location == NULL) {
                    free(new);
                    break;
                }
                new->filename = NULL;
                new->length = -1;
                new->size = -1;
                new->age = -1;
                new->access = -1;
                new->last_modified = -1;
                new->expires = -1;
                new->next = q;
                if(p)
                    p->next = new;
                else
                    from = new;
            }
        }
        p = q;
        q = q->next;
    }
    return from;
}