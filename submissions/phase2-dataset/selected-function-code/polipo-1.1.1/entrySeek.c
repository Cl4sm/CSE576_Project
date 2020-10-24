static int
entrySeek(DiskCacheEntryPtr entry, off_t offset)
{
    off_t rc;

    CHECK_ENTRY(entry);
    assert(entry != &negativeEntry);
    if(entry->offset == offset)
        return 1;
    if(offset > entry->body_offset) {
        /* Avoid extending the file by mistake */
        if(entry->size < 0)
            diskEntrySize(entry->object);
        if(entry->size < 0)
            return -1;
        if(entry->size + entry->body_offset < offset)
            return -1;
    }
    rc = lseek(entry->fd, offset, SEEK_SET);
    if(rc < 0) {
        do_log_error(L_ERROR, errno, "Couldn't seek");
        entry->offset = -1;
        return -1;
    }
    entry->offset = offset;
    return 1;
}