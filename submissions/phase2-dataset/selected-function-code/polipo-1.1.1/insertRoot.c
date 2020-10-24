DiskObjectPtr
insertRoot(DiskObjectPtr from, const char *root)
{
    DiskObjectPtr p;

    p = from;
    while(p) {
        if(strcmp(root, p->location) == 0)
            return from;
        p = p->next;
    }

    p = malloc(sizeof(DiskObjectRec));
    if(!p) return from;
    p->location = strdup(root);
    if(p->location == NULL) {
        free(p);
        return from;
    }
    p->filename = NULL;
    p->length = -1;
    p->size = -1;
    p->age = -1;
    p->access = -1;
    p->last_modified = -1;
    p->expires = -1;
    p->next = from;
    return p;
}