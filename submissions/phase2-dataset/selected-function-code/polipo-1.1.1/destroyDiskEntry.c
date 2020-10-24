int
destroyDiskEntry(ObjectPtr object, int d)
{
    DiskCacheEntryPtr entry = object->disk_entry;
    int rc, urc = 1;

    assert(!entry || !entry->local || !d);

    if(d && !entry)
        entry = makeDiskEntry(object, 0);

    CHECK_ENTRY(entry);

    if(!entry || entry == &negativeEntry) {
        return 1;
    }

    assert(entry->object == object);

    if(maxDiskCacheEntrySize >= 0 && object->size > maxDiskCacheEntrySize) {
        /* See writeoutToDisk */
        d = 1;
    }

    if(d) {
        entry->object->flags &= ~OBJECT_DISK_ENTRY_COMPLETE;
        if(entry->filename) {
            urc = unlink(entry->filename);
            if(urc < 0)
                do_log_error(L_WARN, errno, 
                             "Couldn't unlink %s", scrub(entry->filename));
        }
    } else {
        if(entry && entry->metadataDirty)
            writeoutMetadata(object);
        makeDiskEntry(object, 0);
        /* rewriteDiskEntry may change the disk entry */
        entry = object->disk_entry;
        if(entry == NULL || entry == &negativeEntry)
            return 0;
        if(diskCacheWriteoutOnClose > 0) {
            reallyWriteoutToDisk(object, -1, diskCacheWriteoutOnClose);
            entry = object->disk_entry;
            if(entry == NULL || entry == &negativeEntry)
                return 0;
        }
    }
 again:
    rc = close(entry->fd);
    if(rc < 0 && errno == EINTR)
        goto again;

    entry->fd = -1;

    if(entry->filename)
        free(entry->filename);
    entry->filename = NULL;

    if(entry->previous)
        entry->previous->next = entry->next;
    else
        diskEntries = entry->next;
    if(entry->next)
        entry->next->previous = entry->previous;
    else
        diskEntriesLast = entry->previous;

    numDiskEntries--;
    assert(numDiskEntries >= 0);

    free(entry);
    object->disk_entry = NULL;
    if(urc < 0)
        return -1;
    else
        return 1;
}