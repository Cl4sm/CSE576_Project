DiskObjectPtr
readDiskObject(char *filename, struct stat *sb)
{
    int fd, rc, n, dummy, code;
    int length, size;
    time_t date, last_modified, age, atime, expires;
    char *location = NULL, *fn = NULL;
    DiskObjectPtr dobject;
    char *buf;
    int buf_is_chunk, bufsize;
    int body_offset;
    struct stat ss;

    fd = -1;

    if(sb == NULL) {
        rc = stat(filename, &ss);
        if(rc < 0) {
            do_log_error(L_WARN, errno, "Couldn't stat %s", scrub(filename));
            return NULL;
        }
        sb = &ss;
    }

    buf_is_chunk = 1;
    bufsize = CHUNK_SIZE;
    buf = get_chunk();
    if(buf == NULL) {
        do_log(L_ERROR, "Couldn't allocate buffer.\n");
        return NULL;
    }

    if(S_ISREG(sb->st_mode)) {
        fd = open(filename, O_RDONLY | O_BINARY);
        if(fd < 0)
            goto fail;
    again:
        rc = read(fd, buf, bufsize);
        if(rc < 0)
            goto fail;
        
        n = findEndOfHeaders(buf, 0, rc, &dummy);
        if(n < 0) {
            long lrc;
            if(buf_is_chunk) {
                dispose_chunk(buf);
                buf_is_chunk = 0;
                bufsize = bigBufferSize;
                buf = malloc(bigBufferSize);
                if(buf == NULL)
                    goto fail2;
                lrc = lseek(fd, 0, SEEK_SET);
                if(lrc < 0)
                    goto fail;
                goto again;
            }
            goto fail;
        }
        
        rc = httpParseServerFirstLine(buf, &code, &dummy, NULL);
        if(rc < 0)
            goto fail;

        rc = httpParseHeaders(0, NULL, buf, rc, NULL,
                              NULL, &length, NULL, NULL, NULL, 
                              &date, &last_modified, &expires, &age,
                              &atime, &body_offset, NULL,
                              NULL, NULL, NULL, NULL, &location, NULL, NULL);
        if(rc < 0 || location == NULL)
            goto fail;
        if(body_offset < 0)
            body_offset = n;
    
        size = sb->st_size - body_offset;
        if(size < 0)
            size = 0;
    } else if(S_ISDIR(sb->st_mode)) {
        char *n;
        n = dirnameUrl(buf, 512, (char*)filename, strlen(filename));
        if(n == NULL)
            goto fail;
        location = strdup(n);
        if(location == NULL)
            goto fail;
        length = -1;
        size = -1;
        body_offset = -1;
        age = -1;
        atime = -1;
        date = -1;
        last_modified = -1;
        expires = -1;
    } else {
        goto fail;
    }

    dobject = malloc(sizeof(DiskObjectRec));
    if(!dobject)
        goto fail;
    
    fn = strdup(filename);
    if(!fn)
        goto fail;

    if(buf_is_chunk)
        dispose_chunk(buf);
    else
        free(buf);

    dobject->location = location;
    dobject->filename = fn;
    dobject->length = length;
    dobject->body_offset = body_offset;
    dobject->size = size;
    dobject->age = age;
    dobject->access = atime;
    dobject->date = date;
    dobject->last_modified = last_modified;
    dobject->expires = expires;
    if(fd >= 0) close(fd);
    return dobject;

 fail:
    if(buf_is_chunk)
        dispose_chunk(buf);
    else
        free(buf);
 fail2:
    if(fd >= 0) close(fd);
    if(location) free(location);
    return NULL;
}    