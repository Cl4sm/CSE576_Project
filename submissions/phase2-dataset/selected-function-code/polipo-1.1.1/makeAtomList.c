AtomListPtr
makeAtomList(AtomPtr *atoms, int n)
{
    AtomListPtr list;
    list = malloc(sizeof(AtomListRec));
    if(list == NULL) return NULL;
    list->length = 0;
    list->size = 0;
    list->list = NULL;
    if(n > 0) {
        int i;
        list->list = malloc(n * sizeof(AtomPtr));
        if(list->list == NULL) {
            free(list);
            return NULL;
        }
        list->size = n;
        for(i = 0; i < n; i++)
            list->list[i] = atoms[i];
        list->length = n;
    }
    return list;
}