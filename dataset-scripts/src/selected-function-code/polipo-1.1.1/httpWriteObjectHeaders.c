int
httpWriteObjectHeaders(char *buf, int offset, int len,
                       ObjectPtr object, int from, int to)
{
    int n = offset;
    CacheControlRec cache_control;

    cache_control.flags = object->cache_control;
    cache_control.max_age = object->max_age;
    cache_control.s_maxage = object->s_maxage;
    cache_control.max_stale = -1;
    cache_control.min_fresh = -1;

    if(from <= 0 && to < 0) {
        if(object->length >= 0) {
            n = snnprintf(buf, n, len,
                          "\r\nContent-Length: %d", object->length);
        }
    } else {
        if(to >= 0) {
            n = snnprintf(buf, n, len,
                          "\r\nContent-Length: %d", to - from);
        }
    }

    if(from > 0 || to > 0) {
        if(object->length >= 0) {
            if(from >= to) {
                n = snnprintf(buf, n, len,
                              "\r\nContent-Range: bytes */%d",
                              object->length);
            } else {
                n = snnprintf(buf, n, len,
                              "\r\nContent-Range: bytes %d-%d/%d",
                              from, to - 1, 
                              object->length);
            }
        } else {
            if(to >= 0) {
                n = snnprintf(buf, n, len,
                              "\r\nContent-Range: bytes %d-/*",
                              from);
            } else {
                n = snnprintf(buf, n, len,
                              "\r\nContent-Range: bytes %d-%d/*",
                              from, to);
            }
        }
    }
        
    if(object->etag) {
        n = snnprintf(buf, n, len, "\r\nETag: \"%s\"", object->etag);
    }
    if((object->flags & OBJECT_LOCAL) || object->date >= 0) {
        n = snnprintf(buf, n, len, "\r\nDate: ");
        n = format_time(buf, n, len, 
                        (object->flags & OBJECT_LOCAL) ?
                        current_time.tv_sec : object->date);
        if(n < 0)
            goto fail;
    }

    if(object->last_modified >= 0) {
        n = snnprintf(buf, n, len, "\r\nLast-Modified: ");
        n = format_time(buf, n, len, object->last_modified);
        if(n < 0)
            goto fail;
    }

    if(object->expires >= 0) {
        n = snnprintf(buf, n, len, "\r\nExpires: ");
        n = format_time(buf, n, len, object->expires);
        if(n < 0)
            goto fail;
    }

    n = httpPrintCacheControl(buf, n, len,
                              object->cache_control, &cache_control);
    if(n < 0)
        goto fail;

    if(!disableVia && object->via)
        n = snnprintf(buf, n, len, "\r\nVia: %s", object->via->string);

    if(object->headers)
        n = snnprint_n(buf, n, len, object->headers->string,
                       object->headers->length);

    if(n < len)
        return n;
    else
        return -1;

 fail:
    return -1;
}