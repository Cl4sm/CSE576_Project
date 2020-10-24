static int
insertRange(int from, int to, IntListPtr list, int i)
{
    assert(i >= 0 && i <= list->length);
    assert(i == 0 || list->ranges[i - 1].to < from - 1);
    assert(i == list->length || list->ranges[i].from > to + 1);

    if(list->length >= list->size) {
        int newsize = list->size * 2 + 1;
        IntRangePtr newranges = 
            realloc(list->ranges, newsize * sizeof(IntRangeRec));
        if(newranges == NULL)
            return -1;
        list->size = newsize;
        list->ranges = newranges;
    }

    if(i < list->length)
        memmove(list->ranges + i + 1, list->ranges + i,
                list->length - i);
    list->length++;
    list->ranges[i].from = from;
    list->ranges[i].to = to;
    return 1;
}