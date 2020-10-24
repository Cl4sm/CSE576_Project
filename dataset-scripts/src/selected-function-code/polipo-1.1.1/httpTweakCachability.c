void
httpTweakCachability(ObjectPtr object)
{
    int code = object->code;

    if((object->cache_control & CACHE_AUTHORIZATION) &&
       !(object->cache_control & CACHE_PUBLIC)) {
        object->cache_control |= CACHE_NO_HIDDEN;
        object->flags |= OBJECT_LINEAR;
    }

    /* This is not required by RFC 2616 -- but see RFC 3143 2.1.1.  We
       manically avoid caching replies that we don't know how to
       handle, even if Expires or Cache-Control says otherwise.  As to
       known uncacheable replies, we obey Cache-Control and default to
       allowing sharing but not caching. */
    if(code != 200 && code != 206 && 
       code != 300 && code != 301 && code != 302 && code != 303 &&
       code != 304 && code != 307 &&
       code != 403 && code != 404 && code != 405 && code != 416) {
        object->cache_control |= (CACHE_NO_HIDDEN | CACHE_MISMATCH);
        object->flags |= OBJECT_LINEAR;
    } else if(code != 200 && code != 206 &&
              code != 300 && code != 301 && code != 304 &&
              code != 410) {
        if(object->expires < 0 && !(object->cache_control & CACHE_PUBLIC)) {
            object->cache_control |= CACHE_NO_HIDDEN;
        }
    } else if(dontCacheRedirects && (code == 301 || code == 302)) {
        object->cache_control |= CACHE_NO_HIDDEN;
    }

    if(urlIsUncachable(object->key, object->key_size)) {
        object->cache_control |= CACHE_NO_HIDDEN;
    }

    if((object->cache_control & CACHE_NO_STORE) != 0) {
        object->cache_control |= CACHE_NO_HIDDEN;
    }

    if(object->cache_control & CACHE_VARY) {
        if(!object->etag || dontTrustVaryETag >= 2) {
            object->cache_control |= CACHE_MISMATCH;
        }
    }
}