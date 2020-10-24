static int 
reallyWriteoutToDisk(ObjectPtr object, int upto, int max)
{
    DiskCacheEntryPtr entry;
    int rc;
    int i, j;
    int offset;
    int bytes = 0;

    if(upto < 0)
        upto = object->size;

    if((object->cache_control & CACHE_NO_STORE) || 
       (object->flags & OBJECT_LOCAL))
        return 0;

    if((object->flags & OBJECT_DISK_ENTRY_COMPLETE) && !object->disk_entry)
        return 0;

    entry = makeDiskEntry(object, 1);
    if(!entry) return 0;

    assert(!entry->local);

    if(object->flags & OBJECT_DISK_ENTRY_COMPLETE)
        goto done;

    diskEntrySize(object);
    if(entry->size < 0)
        return 0;

    if(object->length >= 0 && entry->size >= object->length) {
        object->flags |= OBJECT_DISK_ENTRY_COMPLETE;
        goto done;
    }

    if(entry->size >= upto)
        goto done;

    offset = entry->size;

    /* Avoid a seek in case we start writing at the beginning */
    if(offset == 0 && entry->metadataDirty) {
        writeoutMetadata(object);
        /* rewriteDiskEntry may change the entry */
        entry = makeDiskEntry(object, 0);
        if(entry == NULL)
            return 0;
    }

    rc = entrySeek(entry, offset + entry->body_offset);
    if(rc < 0) return 0;

    do {
        if(max >= 0 && bytes >= max)
            break;
        CHECK_ENTRY(entry);
        assert(entry->offset == offset + entry->body_offset);
        i = offset / CHUNK_SIZE;
        j = offset % CHUNK_SIZE;
        if(i >= object->numchunks)
            break;
        if(object->chunks[i].size <= j)
            break;
    again:
        rc = write(entry->fd, object->chunks[i].data + j,
                   object->chunks[i].size - j);
        if(rc < 0) {
            if(errno == EINTR)
                goto again;
            do_log_error(L_ERROR, errno, "Couldn't write disk entry");
            break;
        }
        entry->offset += rc;
        offset += rc;
        bytes += rc;
        if(entry->size < offset)
            entry->size = offset;
    } while(j + rc >= CHUNK_SIZE);

 done:
    CHECK_ENTRY(entry);
    if(entry->metadataDirty)
        writeoutMetadata(object);

    return bytes;
}