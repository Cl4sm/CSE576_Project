int
diskEntrySize(ObjectPtr object)
{
    struct stat buf;
    int rc;
    DiskCacheEntryPtr entry = object->disk_entry;

    if(!entry || entry == &negativeEntry)
        return -1;

    if(entry->size >= 0)
        return entry->size;

    rc = fstat(entry->fd, &buf);
    if(rc < 0) {
        do_log_error(L_ERROR, errno, "Couldn't stat");
        return -1;
    }

    if(buf.st_size <= entry->body_offset)
        entry->size = 0;
    else
        entry->size =  buf.st_size - entry->body_offset;
    CHECK_ENTRY(entry);
    if(object->length >= 0 && entry->size == object->length)
        object->flags |= OBJECT_DISK_ENTRY_COMPLETE;
    return entry->size;
}