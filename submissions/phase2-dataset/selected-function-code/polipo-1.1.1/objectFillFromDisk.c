int 
objectFillFromDisk(ObjectPtr object, int offset, int chunks)
{
    DiskCacheEntryPtr entry;
    int rc, result;
    int i, j, k;
    int complete;

    if(object->type != OBJECT_HTTP)
        return 0;

    if(object->flags & OBJECT_LINEAR)
        return 0;

    if(object->length >= 0) {
        chunks = MIN(chunks, 
                     (object->length - offset + CHUNK_SIZE - 1) / CHUNK_SIZE);
    }

    rc = objectSetChunks(object, offset / CHUNK_SIZE + chunks);
    if(rc < 0)
        return 0;

    complete = 1;
    if(object->flags & OBJECT_INITIAL) {
        complete = 0;
    } else if((object->length < 0 || object->size < object->length) &&
              object->size < (offset / CHUNK_SIZE + chunks) * CHUNK_SIZE) {
        complete = 0;
    } else {
        for(k = 0; k < chunks; k++) {
            int s;
            i = offset / CHUNK_SIZE + k;
            s = MIN(CHUNK_SIZE, object->size - i * CHUNK_SIZE);
            if(object->chunks[i].size < s) {
                complete = 0;
                break;
            }
        }
    }

    if(complete)
        return 1;

    /* This has the side-effect of revalidating the entry, which is
       what makes HEAD requests work. */
    entry = makeDiskEntry(object, 0);
    if(!entry)
        return 0;
                
    for(k = 0; k < chunks; k++) {
        i = offset / CHUNK_SIZE + k;
        if(!object->chunks[i].data)
            object->chunks[i].data = get_chunk();
        if(!object->chunks[i].data) {
            chunks = k;
            break;
        }
        lockChunk(object, i);
    }

    result = 0;

    for(k = 0; k < chunks; k++) {
        int o;
        i = offset / CHUNK_SIZE + k;
        j = object->chunks[i].size;
        o = i * CHUNK_SIZE + j;

        if(object->chunks[i].size == CHUNK_SIZE)
            continue;

        if(entry->size >= 0 && entry->size <= o)
            break;

        if(entry->offset != entry->body_offset + o) {
            rc = entrySeek(entry, entry->body_offset + o);
            if(rc < 0) {
                result = 0;
                break;
            }
        }

        CHECK_ENTRY(entry);
        again:
        rc = read(entry->fd, object->chunks[i].data + j, CHUNK_SIZE - j);
        if(rc < 0) {
            if(errno == EINTR)
                goto again;
            entry->offset = -1;
            do_log_error(L_ERROR, errno, "Couldn't read");
            break;
        }

        entry->offset += rc;
        object->chunks[i].size += rc;
        if(object->size < o + rc)
            object->size = o + rc;

        if(entry->object->length >= 0 && entry->size < 0 &&
           entry->offset - entry->body_offset == entry->object->length)
            entry->size = entry->object->length;
            
        if(rc < CHUNK_SIZE - j) {
            /* Paranoia: the read may have been interrupted half-way. */
            if(entry->size < 0) {
                if(rc == 0 ||
                   (entry->object->length >= 0 &&
                    entry->object->length == 
                    entry->offset - entry->body_offset))
                    entry->size = entry->offset - entry->body_offset;
                break;
            } else if(entry->size != entry->offset - entry->body_offset) {
                if(rc == 0 || 
                   entry->size < entry->offset - entry->body_offset) {
                    do_log(L_WARN,
                           "Disk entry size changed behind our back: "
                           "%ld -> %ld (%d).\n",
                           (long)entry->size,
                           (long)entry->offset - entry->body_offset,
                           object->size);
                    entry->size = -1;
                }
            }
            break;
        }

        CHECK_ENTRY(entry);
        result = 1;
    }

    CHECK_ENTRY(object->disk_entry);
    for(k = 0; k < chunks; k++) {
        i = offset / CHUNK_SIZE + k;
        unlockChunk(object, i);
    }

    if(result > 0) {
        notifyObject(object);
        return 1;
    } else {
        return 0;
    }
}