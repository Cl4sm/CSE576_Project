void
atomListCons(AtomPtr atom, AtomListPtr list)
{
    if(list->list == NULL) {
        assert(list->size == 0);
        list->list = malloc(5 * sizeof(AtomPtr));
        if(list->list == NULL) {
            do_log(L_ERROR, "Couldn't allocate AtomList\n");
            return;
        }
        list->size = 5;
    }
    if(list->size <= list->length) {
        AtomPtr *new_list;
        int n = (2 * list->length + 1);
        new_list = realloc(list->list, n * sizeof(AtomPtr));
        if(new_list == NULL) {
            do_log(L_ERROR, "Couldn't realloc AtomList\n");
            return;
        }
        list->list = new_list;
        list->size = n;
    }
    list->list[list->length] = atom;
    list->length++;
}