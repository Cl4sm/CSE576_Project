int
objectAddData(ObjectPtr object, const char *data, int offset, int len)
{
    int rc;

    do_log(D_OBJECT_DATA, "Adding data to 0x%lx (%d) at %d: %d bytes\n",
           (unsigned long)object, object->length, offset, len);

    if(len == 0)
        return 1;

    if(object->length >= 0) {
        if(offset + len > object->length) {
            do_log(L_ERROR, 
                   "Inconsistent object length (%d, should be at least %d).\n",
                   object->length, offset + len);
            object->length = offset + len;
        }
    }
            
    object->flags &= ~OBJECT_FAILED;

    if(offset + len >= object->numchunks * CHUNK_SIZE) {
        rc = objectSetChunks(object, (offset + len - 1) / CHUNK_SIZE + 1);
        if(rc < 0) {
            return -1;
        }
    }

    if(offset % CHUNK_SIZE != 0) {
        int plen = CHUNK_SIZE - offset % CHUNK_SIZE;
        if(plen >= len)
            plen = len;
        rc = objectAddChunkEnd(object, data, offset, plen);
        if(rc < 0) {
            return -1;
        }            
        offset += plen;
        data += plen;
        len -= plen;
    }

    while(len > 0) {
        int plen = (len >= CHUNK_SIZE) ? CHUNK_SIZE : len;
        rc = objectAddChunk(object, data, offset, plen);
        if(rc < 0) {
            return -1;
        }
        offset += plen;
        data += plen;
        len -= plen;
    }

    return 1;
}