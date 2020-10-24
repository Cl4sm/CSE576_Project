static int
rewriteEntry(ObjectPtr object)
{
    int old_body_offset = object->disk_entry->body_offset;
    int fd, rc, n;
    DiskCacheEntryPtr entry;
    char* buf;
    int buf_is_chunk, bufsize;
    int offset;

    fd = dup(object->disk_entry->fd);
    if(fd < 0) {
        do_log_error(L_ERROR, errno, "Couldn't duplicate file descriptor");
        return -1;
    }

    rc = destroyDiskEntry(object, 1);
    if(rc < 0) {
        close(fd);
        return -1;
    }
    entry = makeDiskEntry(object, 1);
    if(!entry) {
        close(fd);
        return -1;
    }

    offset = diskEntrySize(object);
    if(offset < 0) {
        close(fd);
        return -1;
    }

    bufsize = CHUNK_SIZE;
    buf_is_chunk = 1;
    buf = maybe_get_chunk();
    if(!buf) {
        bufsize = 2048;
        buf_is_chunk = 0;
        buf = malloc(2048);
        if(buf == NULL) {
            do_log(L_ERROR, "Couldn't allocate buffer.\n");
            close(fd);
            return -1;
        }
    }

    rc = lseek(fd, old_body_offset + offset, SEEK_SET);
    if(rc < 0)
        goto done;

    while(1) {
        CHECK_ENTRY(entry);
        n = read(fd, buf, bufsize);
        if(n < 0 && errno == EINTR)
            continue;
        if(n <= 0)
            goto done;
        rc = entrySeek(entry, entry->body_offset + offset);
        if(rc < 0)
            goto done;
    write_again:
        rc = write(entry->fd, buf, n);
        if(rc >= 0) {
            entry->offset += rc;
            entry->size += rc;
        } else if(errno == EINTR) {
            goto write_again;
        }
        if(rc < n)
            goto done;
    }

 done:
    CHECK_ENTRY(entry);
    if(object->length >= 0 && entry->size == object->length)
        object->flags |= OBJECT_DISK_ENTRY_COMPLETE;
    close(fd);
    if(buf_is_chunk)
        dispose_chunk(buf);
    else
        free(buf);
    return 1;
}