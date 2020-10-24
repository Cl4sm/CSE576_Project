int 
writeoutMetadata(ObjectPtr object)
{
    DiskCacheEntryPtr entry;
    int rc;

    if((object->cache_control & CACHE_NO_STORE) || 
       (object->flags & OBJECT_LOCAL))
        return 0;
    
    entry = makeDiskEntry(object, 0);
    if(entry == NULL || entry == &negativeEntry)
        goto fail;

    assert(!entry->local);

    rc = entrySeek(entry, 0);
    if(rc < 0) goto fail;

    rc = writeHeaders(entry->fd, &entry->body_offset, object, NULL, 0);
    if(rc == -2) {
        rc = rewriteEntry(object);
        if(rc < 0) return 0;
        return 1;
    }
    if(rc < 0) goto fail;
    entry->offset = rc;
    entry->metadataDirty = 0;
    return 1;

 fail:
    /* We need this in order to avoid trying to write this entry out
       multiple times. */
    if(entry && entry != &negativeEntry)
        entry->metadataDirty = 0;
    return 0;
}