int
httpWriteRequest(HTTPConnectionPtr connection, HTTPRequestPtr request,
                 int bodylen)
{
    ObjectPtr object = request->object;
    int from = request->from, to = request->to, method = request->method;
    char *url = object->key, *m;
    int url_size = object->key_size;
    int x, y, port, z, location_size;
    char *location;
    int l, n, rc, bufsize;

    assert(method != METHOD_NONE);

    if(request->method == METHOD_GET || 
       request->method == METHOD_CONDITIONAL_GET) {
        if(to >= 0) {
            assert(to >= from);
            if(to == from) {
                do_log(L_ERROR, "Requesting empty segment?\n");
                return -1;
            }
        }

        if(object->flags & OBJECT_DYNAMIC) {
            from = 0;
            to = -1;
        } else {
            objectFillFromDisk(object, from / CHUNK_SIZE * CHUNK_SIZE, 1);
            l = objectHoleSize(request->object, from);
            if(l > 0) {
                if(to <= 0 || to > from + l)
                    to = from + l;
            }

            if(pmmSize && connection->server->pipeline >= 4) {
                if(from == 0)
                    to = to < 0 ? pmmFirstSize : MIN(to, pmmFirstSize);
                else
                    to = to < 0 ? from + pmmSize : MIN(to, from + pmmSize);
            }

            if(from % CHUNK_SIZE != 0)
                if(objectHoleSize(object, from - 1) != 0)
                    from = from / CHUNK_SIZE * CHUNK_SIZE;
        }
    }

    rc = parseUrl(url, url_size, &x, &y, &port, &z);

    if(rc < 0 || x < 0 || y < 0) {
        return -1;
    }

    if(connection->reqbuf == NULL) {
        connection->reqbuf = get_chunk();
        if(connection->reqbuf == NULL)
            return -1;
        connection->reqlen = 0;
    }

    if(method == METHOD_CONDITIONAL_GET &&
       object->last_modified < 0 && object->etag == NULL)
        method = request->method = METHOD_GET;

 again:
    bufsize = 
        (connection->flags & CONN_BIGREQBUF) ? bigBufferSize : CHUNK_SIZE;
    n = connection->reqlen;
    switch(method) {
    case METHOD_GET:
    case METHOD_CONDITIONAL_GET: m = "GET"; break;
    case METHOD_HEAD: m = "HEAD"; break;
    case METHOD_POST: m = "POST"; break;
    case METHOD_PUT: m = "PUT"; break;
    default: abort();
    }
    n = snnprintf(connection->reqbuf, n, bufsize, "%s ", m);

    if(connection->server->isProxy) {
        n = snnprint_n(connection->reqbuf, n, bufsize,
                       url, url_size);
    } else {
        if(url_size - z == 0) {
            location = "/";
            location_size = 1;
        } else {
            location = url + z;
            location_size = url_size - z;
        }
        
        n = snnprint_n(connection->reqbuf, n, bufsize, 
                       location, location_size);
    }
    
    do_log(D_SERVER_REQ, "Server request: ");
    do_log_n(D_SERVER_REQ, url + x, y - x);
    do_log(D_SERVER_REQ, ": ");
    do_log_n(D_SERVER_REQ, connection->reqbuf, n);
    do_log(D_SERVER_REQ, " (method %d from %d to %d, 0x%lx for 0x%lx)\n",
           method, from, to,
           (unsigned long)connection, (unsigned long)object);

    n = snnprintf(connection->reqbuf, n, bufsize, " HTTP/1.1");

    n = snnprintf(connection->reqbuf, n, bufsize, "\r\nHost: ");
    n = snnprint_n(connection->reqbuf, n, bufsize, url + x, y - x);
    if(port != 80)
        n = snnprintf(connection->reqbuf, n, bufsize, ":%d", port);

    if(connection->server->isProxy && parentAuthCredentials) {
        n = buildServerAuthHeaders(connection->reqbuf, n, bufsize,
                                   parentAuthCredentials);
    }

    if(bodylen >= 0)
        n = snnprintf(connection->reqbuf, n, bufsize,
                      "\r\nContent-Length: %d", bodylen);

    if(request->flags & REQUEST_WAIT_CONTINUE)
        n = snnprintf(connection->reqbuf, n, bufsize,
                      "\r\nExpect: 100-continue");

    if(method != METHOD_HEAD && (from > 0 || to >= 0)) {
        if(to >= 0) {
            n = snnprintf(connection->reqbuf, n, bufsize,
                          "\r\nRange: bytes=%d-%d", from, to - 1);
        } else {
            n = snnprintf(connection->reqbuf, n, bufsize,
                          "\r\nRange: bytes=%d-", from);
        }
    }

    if(method == METHOD_GET && object->etag && (from > 0 || to >= 0)) {
        if(request->request && request->request->request == request &&
           request->request->from == 0 && request->request->to == -1 &&
           pmmSize == 0 && pmmFirstSize == 0)
            n = snnprintf(connection->reqbuf, n, bufsize,
                          "\r\nIf-Range: \"%s\"", object->etag);
    }

    if(method == METHOD_CONDITIONAL_GET) {
        if(object->last_modified >= 0) {
            n = snnprintf(connection->reqbuf, n, bufsize,
                          "\r\nIf-Modified-Since: ");
            n = format_time(connection->reqbuf, n, bufsize,
                            object->last_modified);
        }
        if(object->etag) {
            n = snnprintf(connection->reqbuf, n, bufsize,
                          "\r\nIf-None-Match: \"%s\"", object->etag);
        }
    }

    n = httpPrintCacheControl(connection->reqbuf, n, bufsize,
                              alwaysAddNoTransform ? CACHE_NO_TRANSFORM : 0,
			      &request->cache_control);
    if(n < 0)
        goto fail;

    if(request->request && request->request->headers) {
        n = snnprint_n(connection->reqbuf, n, bufsize,
                       request->request->headers->string, 
                       request->request->headers->length);
    }
    if(!disableVia) {
        if(request->request && request->request->via) {
            n = snnprintf(connection->reqbuf, n, bufsize,
                          "\r\nVia: %s, 1.1 %s",
                          request->request->via->string, proxyName->string);
        } else {
            n = snnprintf(connection->reqbuf, n, bufsize,
                          "\r\nVia: 1.1 %s",
                          proxyName->string);
        }
    }

    n = snnprintf(connection->reqbuf, n, bufsize,
                  "\r\nConnection: %s\r\n\r\n",
                  (request->flags & REQUEST_PERSISTENT) ? 
                  "keep-alive" : "close");
    if(n < 0 || n >= bufsize - 1)
        goto fail;
    connection->reqlen = n;
    return n;

 fail:
    rc = 0;
    if(!(connection->flags & CONN_BIGREQBUF))
        rc = httpConnectionBigifyReqbuf(connection);
    if(rc == 1)
        goto again;
    return -1;
}