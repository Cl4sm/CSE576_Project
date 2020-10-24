int
objectHasData(ObjectPtr object, int from, int to)
{
    int first, last, i, upto;

    if(to < 0) {
        if(object->length >= 0)
            to = object->length;
        else
            return 0;
    }

    first = from / CHUNK_SIZE;
    last = to / CHUNK_SIZE;

    if(from >= to)
        return 2;

    if(to > object->size) {
        upto = to;
        goto disk;
    }

    if(last > object->numchunks ||
       object->chunks[last].size > to % CHUNK_SIZE) {
        upto = to;
        goto disk;
    }

    for(i = last - 1; i >= first; i--) {
        if(object->chunks[i].size < CHUNK_SIZE) {
            upto = (i + 1) * CHUNK_SIZE;
            goto disk;
        }
    }

    return 2;

 disk:
    if(object->flags & OBJECT_DISK_ENTRY_COMPLETE)
        return 1;

    if(diskEntrySize(object) >= upto)
        return 1;

    return 0;
}