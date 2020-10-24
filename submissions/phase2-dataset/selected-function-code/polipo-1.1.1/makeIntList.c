IntListPtr
makeIntList(int size)
{
    IntListPtr list;
    if(size <= 0)
        size = 4;

    list = malloc(sizeof(IntListRec));
    if(list == NULL)
        return NULL;

    list->ranges = malloc(size * sizeof(IntRangeRec));
    if(list->ranges == NULL) {
        free(list);
        return NULL;
    }

    list->length = 0;
    list->size = size;
    return list;
}