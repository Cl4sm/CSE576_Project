static int
objectAddChunk(ObjectPtr object, const char *data, int offset, int plen)
{
    int i = offset / CHUNK_SIZE;
    int rc;

    assert(offset % CHUNK_SIZE == 0);
    assert(plen <= CHUNK_SIZE);

    if(object->numchunks <= i) {
        rc = objectSetChunks(object, i + 1);
        if(rc < 0)
            return -1;
    }

    lockChunk(object, i);

    if(object->chunks[i].data == NULL) {
        object->chunks[i].data = get_chunk();
        if(object->chunks[i].data == NULL)
            goto fail;
    }

    if(object->chunks[i].size >= plen) {
        unlockChunk(object, i);
        return 0;
    }

    if(object->size < offset + plen)
        object->size = offset + plen;
    object->chunks[i].size = plen;
    memcpy(object->chunks[i].data, data, plen);
    unlockChunk(object, i);
    return 0;

 fail:
    unlockChunk(object, i);
    return -1;
}