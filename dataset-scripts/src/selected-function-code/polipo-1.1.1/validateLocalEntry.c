int
validateLocalEntry(ObjectPtr object, int fd,
                   int *body_offset_return, off_t *offset_return)
{
    struct stat ss;
    char buf[512];
    int n, rc;
    char *encoding;

    rc = fstat(fd, &ss);
    if(rc < 0) {
        do_log_error(L_ERROR, errno, "Couldn't stat");
        return -1;
    }

    if(S_ISREG(ss.st_mode)) {
        if(!(ss.st_mode & S_IROTH) ||
           (object->length >= 0 && object->length != ss.st_size) ||
           (object->last_modified >= 0 && 
            object->last_modified != ss.st_mtime))
            return -1;
    } else {
        notifyObject(object);
        return -1;
    }
    
    n = snnprintf(buf, 0, 512, "%lx-%lx-%lx",
                  (unsigned long)ss.st_ino,
                  (unsigned long)ss.st_size,
                  (unsigned long)ss.st_mtime);
    if(n >= 512)
        n = -1;

    if(n > 0 && object->etag) {
        if(strlen(object->etag) != n ||
           memcmp(object->etag, buf, n) != 0)
            return -1;
    }

    if(!(object->flags & OBJECT_INITIAL)) {
        if(!object->last_modified && !object->etag)
            return -1;
    }
       
    if(object->flags & OBJECT_INITIAL) {
        object->length = ss.st_size;
        object->last_modified = ss.st_mtime;
        object->date = current_time.tv_sec;
        object->age = current_time.tv_sec;
        object->code = 200;
        if(n > 0)
            object->etag = strdup(buf); /* okay if fails */
        object->message = internAtom("Okay");
        n = snnprintf(buf, 0, 512,
                      "\r\nServer: Polipo"
                      "\r\nContent-Type: %s",
                      localObjectMimeType(object, &encoding));
        if(encoding != NULL)
            n = snnprintf(buf, n, 512,
                          "\r\nContent-Encoding: %s", encoding);
        if(n < 0)
            return -1;
        object->headers = internAtomN(buf, n);
        if(object->headers == NULL)
            return -1;
        object->flags &= ~OBJECT_INITIAL;
    }

    if(body_offset_return)
        *body_offset_return = 0;
    if(offset_return)
        *offset_return = 0;
    return 0;
}