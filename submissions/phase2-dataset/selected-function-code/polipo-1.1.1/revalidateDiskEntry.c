int
revalidateDiskEntry(ObjectPtr object)
{
    DiskCacheEntryPtr entry = object->disk_entry;
    int rc;
    int body_offset;

    if(!entry || entry == &negativeEntry)
        return 1;

    CHECK_ENTRY(entry);
    rc = entrySeek(entry, 0);
    if(rc < 0) return 0;

    rc = validateEntry(object, entry->fd, &body_offset, &entry->offset);
    if(rc < 0) {
        destroyDiskEntry(object, 0);
        return 0;
    }
    if(body_offset != entry->body_offset) {
        do_log(L_WARN, "Inconsistent body offset (%d != %d).\n",
               body_offset, entry->body_offset);
        destroyDiskEntry(object, 0);
        return 0;
    }

    entry->metadataDirty |= !!rc;
    CHECK_ENTRY(entry);
    return 1;
}