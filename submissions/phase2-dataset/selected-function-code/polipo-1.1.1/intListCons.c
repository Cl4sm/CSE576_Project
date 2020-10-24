int
intListCons(int from, int to, IntListPtr list)
{
    int i;

    /* Don't bother with the dichotomy. */
    for(i = 0; i < list->length; i++) {
        if(list->ranges[i].to >= from - 1)
            break;
    }

    if(i < list->length && 
       (from >= list->ranges[i].from - 1 || to <= list->ranges[i].to + 1)) {
        if(from <= list->ranges[i].from)
            list->ranges[i].from = from;
        if(to >= list->ranges[i].to)
            list->ranges[i].to = to;
        return maybeMergeRanges(list, i);
    }
    return insertRange(from, to, list, i);
}