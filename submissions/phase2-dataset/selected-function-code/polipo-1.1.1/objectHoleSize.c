int 
objectHoleSize(ObjectPtr object, int offset)
{
    int size = 0, i;

    if(offset < 0 || offset / CHUNK_SIZE >= object->numchunks)
        return -1;

    if(offset % CHUNK_SIZE != 0) {
        if(object->chunks[offset / CHUNK_SIZE].size > offset % CHUNK_SIZE)
            return 0;
        else {
            size += CHUNK_SIZE - offset % CHUNK_SIZE;
            offset += CHUNK_SIZE - offset % CHUNK_SIZE;
            if(offset < 0) {
                /* Overflow */
                return -1;
            }
        }
    }

    for(i = offset / CHUNK_SIZE; i < object->numchunks; i++) {
        if(object->chunks[i].size == 0)
            size += CHUNK_SIZE;
        else
            break;
    }
    if(i >= object->numchunks)
        return -1;
    return size;
}