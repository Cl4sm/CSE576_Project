static DiskCacheEntryPtr
makeDiskEntry(ObjectPtr object, int create)
{
    DiskCacheEntryPtr entry = NULL;
    char buf[1024];
    int fd = -1;
    int negative = 0, size = -1, name_len = -1;
    char *name = NULL;
    off_t offset = -1;
    int body_offset = -1;
    int rc;
    int local = (object->flags & OBJECT_LOCAL) != 0;
    int dirty = 0;

   if(local && create)
       return NULL;

    if(!local && !(object->flags & OBJECT_PUBLIC))
        return NULL;

    if(maxDiskCacheEntrySize >= 0) {
        if(object->length > 0) {
            if(object->length > maxDiskCacheEntrySize)
                return NULL;
        } else {
            if(object->size > maxDiskCacheEntrySize)
                return NULL;
        }
    }

    if(object->disk_entry) {
        entry = object->disk_entry;
        CHECK_ENTRY(entry);
        if(entry != &negativeEntry) {
            /* We'll keep the entry -- put it at the front. */
            if(entry != diskEntries && entry != &negativeEntry) {
                entry->previous->next = entry->next;
                if(entry->next)
                    entry->next->previous = entry->previous;
                else
                    diskEntriesLast = entry->previous;
                entry->next = diskEntries;
                diskEntries->previous = entry;
                entry->previous = NULL;
                diskEntries = entry;
            }
            return entry;
        } else {
            if(entry == &negativeEntry) {
                negative = 1;
                if(!create) return NULL;
                object->disk_entry = NULL;
            }
            entry = NULL;
            destroyDiskEntry(object, 0);
        }
    }

    if(numDiskEntries > maxDiskEntries)
        destroyDiskEntry(diskEntriesLast->object, 0);

    if(!local) {
        if(diskCacheRoot == NULL || diskCacheRoot->length <= 0)
            return NULL;
        name_len = urlFilename(buf, 1024, object->key, object->key_size);
        if(name_len < 0) return NULL;
        if(!negative)
            fd = open(buf, O_RDWR | O_BINARY);
        if(fd >= 0) {
            rc = validateEntry(object, fd, &body_offset, &offset);
            if(rc >= 0) {
                dirty = rc;
            } else {
                close(fd);
                fd = -1;
                rc = unlink(buf);
                if(rc < 0 && errno != ENOENT) {
                    do_log_error(L_WARN,  errno,
                                 "Couldn't unlink stale disk entry %s", 
                                 scrub(buf));
                    /* But continue -- it's okay to have stale entries. */
                }
            }
        }

        if(fd < 0 && create && name_len > 0 && 
           !(object->flags & OBJECT_INITIAL)) {
            fd = createFile(buf, diskCacheRoot->length);
            if(fd < 0)
                return NULL;

            if(fd >= 0) {
                char *data = NULL;
                int dsize = 0;
                if(object->numchunks > 0) {
                    data = object->chunks[0].data;
                    dsize = object->chunks[0].size;
                }
                rc = writeHeaders(fd, &body_offset, object, data, dsize);
                if(rc < 0) {
                    do_log_error(L_ERROR, errno, "Couldn't write headers");
                    rc = unlink(buf);
                    if(rc < 0 && errno != ENOENT)
                        do_log_error(L_ERROR, errno,
                                     "Couldn't unlink truncated entry %s", 
                                     scrub(buf));
                    close(fd);
                    return NULL;
                }
                assert(rc >= body_offset);
                size = rc - body_offset;
                offset = rc;
                dirty = 0;
            }
        }
    } else {
        /* local */
        if(localDocumentRoot == NULL || localDocumentRoot->length == 0)
            return NULL;

        name_len = 
            localFilename(buf, 1024, object->key, object->key_size);
        if(name_len < 0)
            return NULL;
        fd = open(buf, O_RDONLY | O_BINARY);
        if(fd >= 0) {
            if(validateEntry(object, fd, &body_offset, NULL) < 0) {
                close(fd);
                fd = -1;
            }
        }
        offset = 0;
    }

    if(fd < 0) {
        object->disk_entry = &negativeEntry;
        return NULL;
    }
    assert(body_offset >= 0);

    name = strdup_n(buf, name_len);
    if(name == NULL) {
        do_log(L_ERROR, "Couldn't allocate name.\n");
        close(fd);
        fd = -1;
        return NULL;
    }

    entry = malloc(sizeof(DiskCacheEntryRec));
    if(entry == NULL) {
        do_log(L_ERROR, "Couldn't allocate entry.\n");
        free(name);
        close(fd);
        return NULL;
    }

    entry->filename = name;
    entry->object = object;
    entry->fd = fd;
    entry->body_offset = body_offset;
    entry->local = local;
    entry->offset = offset;
    entry->size = size;
    entry->metadataDirty = dirty;

    entry->next = diskEntries;
    if(diskEntries)
        diskEntries->previous = entry;
    diskEntries = entry;
    if(diskEntriesLast == NULL)
        diskEntriesLast = entry;
    entry->previous = NULL;
    numDiskEntries++;

    object->disk_entry = entry;

    CHECK_ENTRY(entry);
    return entry;
}