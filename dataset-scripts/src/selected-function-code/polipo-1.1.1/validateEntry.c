int
validateEntry(ObjectPtr object, int fd, 
              int *body_offset_return, off_t *offset_return)
{
    char *buf;
    int buf_is_chunk, bufsize;
    int rc, n;
    int dummy;
    int code;
    AtomPtr headers;
    time_t date, last_modified, expires, polipo_age, polipo_access;
    int length;
    off_t offset = -1;
    int body_offset;
    char *etag;
    AtomPtr via;
    CacheControlRec cache_control;
    char *location;
    AtomPtr message;
    int dirty = 0;

    if(object->flags & OBJECT_LOCAL)
        return validateLocalEntry(object, fd,
                                  body_offset_return, offset_return);

    if(!(object->flags & OBJECT_PUBLIC) && (object->flags & OBJECT_INITIAL))
        return 0;

    /* get_chunk might trigger object expiry */
    bufsize = CHUNK_SIZE;
    buf_is_chunk = 1;
    buf = maybe_get_chunk();
    if(!buf) {
        bufsize = 2048;
        buf_is_chunk = 0;
        buf = malloc(2048);
        if(buf == NULL) {
            do_log(L_ERROR, "Couldn't allocate buffer.\n");
            return -1;
        }
    }

 again:
    rc = read(fd, buf, bufsize);
    if(rc < 0) {
        if(errno == EINTR)
            goto again;
        do_log_error(L_ERROR, errno, "Couldn't read disk entry");
        goto fail;
    }
    offset = rc;

 parse_again:
    n = findEndOfHeaders(buf, 0, rc, &dummy);
    if(n < 0) {
        char *oldbuf = buf;
        if(bufsize < bigBufferSize) {
            buf = malloc(bigBufferSize);
            if(!buf) {
                do_log(L_ERROR, "Couldn't allocate big buffer.\n");
                goto fail;
            }
            bufsize = bigBufferSize;
            memcpy(buf, oldbuf, offset);
            if(buf_is_chunk)
                dispose_chunk(oldbuf);
            else
                free(oldbuf);
            buf_is_chunk = 0;
        again2:
            rc = read(fd, buf + offset, bufsize - offset);
            if(rc < 0) {
                if(errno == EINTR)
                    goto again2;
                do_log_error(L_ERROR, errno, "Couldn't read disk entry");
                goto fail;
            }
            offset += rc;
            goto parse_again;
        }
        do_log(L_ERROR, "Couldn't parse disk entry.\n");
        goto fail;
    }

    rc = httpParseServerFirstLine(buf, &code, &dummy, &message);
    if(rc < 0) {
        do_log(L_ERROR, "Couldn't parse disk entry.\n");
        goto fail;
    }

    if(object->code != 0 && object->code != code) {
        releaseAtom(message);
        goto fail;
    }

    rc = httpParseHeaders(0, NULL, buf, rc, NULL,
                          &headers, &length, &cache_control, NULL, NULL,
                          &date, &last_modified, &expires, &polipo_age,
                          &polipo_access, &body_offset,
                          NULL, &etag, NULL,
                          NULL, NULL, &location, &via, NULL);
    if(rc < 0) {
        releaseAtom(message);
        goto fail;
    }
    if(body_offset < 0)
        body_offset = n;

    if(!location || strlen(location) != object->key_size ||
       memcmp(location, object->key, object->key_size) != 0) {
        do_log(L_ERROR, "Inconsistent cache file for %s.\n", scrub(location));
        goto invalid;
    }

    if(polipo_age < 0)
        polipo_age = date;

    if(polipo_age < 0) {
        do_log(L_ERROR, "Undated disk entry for %s.\n", scrub(location));
        goto invalid;
    }

    if(!(object->flags & OBJECT_INITIAL)) {
        if((last_modified >= 0) != (object->last_modified >= 0))
            goto invalid;

        if((object->cache_control & CACHE_MISMATCH) ||
           (cache_control.flags & CACHE_MISMATCH))
            goto invalid;

        if(last_modified >= 0 && object->last_modified >= 0 &&
           last_modified != object->last_modified)
            goto invalid;

        if(length >= 0 && object->length >= 0)
            if(length != object->length)
                goto invalid;

        if(!!etag != !!object->etag)
            goto invalid;

        if(etag && object->etag && strcmp(etag, object->etag) != 0)
            goto invalid;

        /* If we don't have a usable ETag, and either CACHE_VARY or we
           don't have a last-modified date, we validate disk entries by
           using their date. */
        if(!(etag && object->etag) &&
           (!(last_modified >= 0 && object->last_modified >= 0) ||
            ((cache_control.flags & CACHE_VARY) ||
             (object->cache_control & CACHE_VARY)))) {
            if(date >= 0 && date != object->date)
                goto invalid;
            if(polipo_age >= 0 && polipo_age != object->age)
                goto invalid;
        }
        if((object->cache_control & CACHE_VARY) && dontTrustVaryETag >= 1) {
            /* Check content-type to work around mod_gzip bugs */
            if(!httpHeaderMatch(atomContentType, object->headers, headers) ||
               !httpHeaderMatch(atomContentEncoding, object->headers, headers))
                goto invalid;
        }
    }

    if(location)
        free(location);

    if(headers) {
        if(!object->headers)
            object->headers = headers;
        else
            releaseAtom(headers);
    }

    if(object->code == 0) {
        object->code = code;
        object->message = retainAtom(message);
    }
    if(object->date <= date)
        object->date = date;
    else 
        dirty = 1;
    if(object->last_modified < 0)
        object->last_modified = last_modified;
    if(object->expires < 0)
        object->expires = expires;
    else if(object->expires > expires)
        dirty = 1;
    if(object->age < 0)
        object->age = polipo_age;
    else if(object->age > polipo_age)
        dirty = 1;
    if(object->atime <= polipo_access)
        object->atime = polipo_access;
    else
        dirty = 1;

    object->cache_control |= cache_control.flags;
    object->max_age = cache_control.max_age;
    object->s_maxage = cache_control.s_maxage;

    if(object->age < 0) object->age = object->date;
    if(object->age < 0) object->age = 0; /* a long time ago */
    if(object->length < 0) object->length = length;
    if(!object->etag)
        object->etag = etag;
    else {
        if(etag)
            free(etag);
    }
    releaseAtom(message);

    if(object->flags & OBJECT_INITIAL) object->via = via;
    object->flags &= ~OBJECT_INITIAL;
    if(offset > body_offset) {
        /* We need to make sure we don't invoke object expiry recursively */
        objectSetChunks(object, 1);
        if(object->numchunks >= 1) {
            if(object->chunks[0].data == NULL)
                object->chunks[0].data = maybe_get_chunk();
            if(object->chunks[0].data)
                objectAddData(object, buf + body_offset,
                              0, MIN(offset - body_offset, CHUNK_SIZE));
        }
    }

    httpTweakCachability(object);

    if(buf_is_chunk)
        dispose_chunk(buf);
    else
        free(buf);
    if(body_offset_return) *body_offset_return = body_offset;
    if(offset_return) *offset_return = offset;
    return dirty;

 invalid:
    releaseAtom(message);
    if(etag) free(etag);
    if(location) free(location);
    if(via) releaseAtom(via);
    /* fall through */

 fail:
    if(buf_is_chunk)
        dispose_chunk(buf);
    else
        free(buf);
    return -1;
}