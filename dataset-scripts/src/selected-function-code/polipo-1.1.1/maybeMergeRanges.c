static int
maybeMergeRanges(IntListPtr list, int i)
{
    int rc;

    while(i > 0 && list->ranges[i].from <= list->ranges[i - 1].to + 1) {
            list->ranges[i - 1].from = 
                MIN(list->ranges[i - 1].from, list->ranges[i].from);
            list->ranges[i - 1].to =
                MAX(list->ranges[i - 1].to, list->ranges[i].to);
            rc = deleteRange(list, i);
            if(rc < 0) return -1;
            i--;
    }

    while(i < list->length - 1 && 
          list->ranges[i].to >= list->ranges[i + 1].from - 1) {
            list->ranges[i + 1].from = 
                MIN(list->ranges[i + 1].from, list->ranges[i].from);
            list->ranges[i - 1].to =
                MAX(list->ranges[i + 1].to, list->ranges[i].to);
            rc = deleteRange(list, i);
            if(rc < 0) return -1;
    }
    return 1;
}