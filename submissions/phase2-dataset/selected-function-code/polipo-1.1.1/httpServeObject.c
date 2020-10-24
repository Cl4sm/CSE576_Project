int 
httpServeObject(HTTPConnectionPtr connection)
{
    HTTPRequestPtr request = connection->request;
    ObjectPtr object = request->object;
    int i = request->from / CHUNK_SIZE;
    int j = request->from % CHUNK_SIZE;
    int n, len, rc;
    int bufsize = CHUNK_SIZE;
    int condition_result;

    object->atime = current_time.tv_sec;
    objectMetadataChanged(object, 0);

    httpSetTimeout(connection, -1);

    if((request->error_code && relaxTransparency <= 0) ||
       object->flags & OBJECT_INITIAL) {
        object->flags &= ~OBJECT_FAILED;
        unlockChunk(object, i);
        if(request->error_code)
            return httpClientRawError(connection,
                                      request->error_code, 
                                      retainAtom(request->error_message), 0);
        else
            return httpClientRawError(connection,
                                      500, internAtom("Object vanished."), 0);
    }

    if(!(object->flags & OBJECT_INPROGRESS) && object->code == 0) {
        if(object->flags & OBJECT_INITIAL) {
            unlockChunk(object, i);
            return httpClientRawError(connection, 503,
                                      internAtom("Error message lost"), 0);
                                      
        } else {
            unlockChunk(object, i);
            do_log(L_ERROR, "Internal proxy error: object has no code.\n");
            return httpClientRawError(connection, 500,
                                      internAtom("Internal proxy error: "
                                                 "object has no code"), 0);
        }
    }

    condition_result = httpCondition(object, request->condition);

    if(condition_result == CONDITION_FAILED) {
        unlockChunk(object, i);
        return httpClientRawError(connection, 412,
                                  internAtom("Precondition failed"), 0);
    } else if(condition_result == CONDITION_NOT_MODIFIED) {
        unlockChunk(object, i);
        return httpClientRawError(connection, 304,
                                  internAtom("Not modified"), 0);
    }

    objectFillFromDisk(object, request->from,
                       (request->method == METHOD_HEAD ||
                        condition_result != CONDITION_MATCH) ? 0 : 1);

    if(((object->flags & OBJECT_LINEAR) &&
        (object->requestor != connection->request)) ||
       ((object->flags & OBJECT_SUPERSEDED) &&
        !(object->flags & OBJECT_LINEAR))) {
        if(request->request) {
            request->request->request = NULL;
            request->request = NULL;
            request->object->requestor = NULL;
        }
        object = makeObject(OBJECT_HTTP,
                            object->key, object->key_size, 1, 0,
                            object->request, NULL);
        if(request->object->requestor == request)
            request->object->requestor = NULL;
        unlockChunk(request->object, i);
        releaseObject(request->object);
        request->object = NULL;
        if(object == NULL) {
            do_log(L_ERROR, "Couldn't allocate object.");
            return httpClientRawError(connection, 501,
                                      internAtom("Couldn't allocate object"), 
                                      1);
        }
        if(urlIsLocal(object->key, object->key_size)) {
            object->flags |= OBJECT_LOCAL;
            object->request = httpLocalRequest;
        }
        request->object = object;
        connection->flags &= ~CONN_WRITER;
        return httpClientNoticeRequest(request, 1);
    }

    if(object->flags & OBJECT_ABORTED) {
        unlockChunk(object, i);
        return httpClientNoticeError(request, object->code, 
                                     retainAtom(object->message));
    }

    if(connection->buf == NULL)
        connection->buf = get_chunk();
    if(connection->buf == NULL) {
        unlockChunk(object, i);
        do_log(L_ERROR, "Couldn't allocate client buffer.\n");
        connection->flags &= ~CONN_WRITER;
        httpClientFinish(connection, 1);
        return 1;
    }

    if(object->length >= 0 && request->to >= object->length)
        request->to = object->length;

    if(request->from > 0 || request->to >= 0) {
        if(request->method == METHOD_HEAD) {
            request->to = request->from;
        } else if(request->to < 0) {
            if(object->length >= 0)
                request->to = object->length;
        }
    }

 again:

    connection->len = 0;

    if((request->from <= 0 && request->to < 0) || 
       request->method == METHOD_HEAD) {
        n = snnprintf(connection->buf, 0, bufsize,
                      "HTTP/1.1 %d %s",
                      object->code, atomString(object->message));
    } else {
        if((object->length >= 0 && request->from >= object->length) ||
           (request->to >= 0 && request->from >= request->to)) {
            unlockChunk(object, i);
            return httpClientRawError(connection, 416,
                                      internAtom("Requested range "
                                                 "not satisfiable"),
                                      0);
        } else {
            n = snnprintf(connection->buf, 0, bufsize,
                          "HTTP/1.1 206 Partial content");
        }
    }

    n = httpWriteObjectHeaders(connection->buf, n, bufsize,
                               object, request->from, request->to);
    if(n < 0)
        goto fail;

    if(request->method != METHOD_HEAD && 
       condition_result != CONDITION_NOT_MODIFIED &&
       request->to < 0 && object->length < 0) {
        if(connection->version == HTTP_11) {
            connection->te = TE_CHUNKED;
            n = snnprintf(connection->buf, n, bufsize,
                          "\r\nTransfer-Encoding: chunked");
        } else {
            request->flags &= ~REQUEST_PERSISTENT;
        }
    }
        
    if(object->age < current_time.tv_sec) {
        n = snnprintf(connection->buf, n, bufsize,
                      "\r\nAge: %d",
                      (int)(current_time.tv_sec - object->age));
    }
    n = snnprintf(connection->buf, n, bufsize,
                  "\r\nConnection: %s",
                  (request->flags & REQUEST_PERSISTENT) ? 
                  "keep-alive" : "close");

    if(!(object->flags & OBJECT_LOCAL)) {
        if((object->flags & OBJECT_FAILED) && !proxyOffline) {
            n = snnprintf(connection->buf, n, bufsize,
                          "\r\nWarning: 111 %s:%d Revalidation failed",
                          proxyName->string, proxyPort);
            if(request->error_code)
                n = snnprintf(connection->buf, n, bufsize,
                              " (%d %s)",
                              request->error_code, 
                              atomString(request->error_message));
            object->flags &= ~OBJECT_FAILED;
        } else if(proxyOffline && 
                  objectMustRevalidate(object, &request->cache_control)) {
            n = snnprintf(connection->buf, n, bufsize,
                          "\r\nWarning: 112 %s:%d Disconnected operation",
                          proxyName->string, proxyPort);
        } else if(objectIsStale(object, &request->cache_control)) {
            n = snnprintf(connection->buf, n, bufsize,
                          "\r\nWarning: 110 %s:%d Object is stale",
                          proxyName->string, proxyPort);
        } else if(object->expires < 0 && object->max_age < 0 &&
                  object->age < current_time.tv_sec - 24 * 3600) {
            n = snnprintf(connection->buf, n, bufsize,
                          "\r\nWarning: 113 %s:%d Heuristic expiration",
                          proxyName->string, proxyPort);
        }
    }

    n = snnprintf(connection->buf, n, bufsize, "\r\n\r\n");
    
    if(n < 0)
        goto fail;
    
    connection->offset = request->from;

    if(request->method == METHOD_HEAD || 
       condition_result == CONDITION_NOT_MODIFIED ||
       (object->flags & OBJECT_ABORTED)) {
        len = 0;
    } else {
        if(i < object->numchunks) {
            if(object->chunks[i].size <= j)
                len = 0;
            else
                len = object->chunks[i].size - j;
        } else {
            len = 0;
        }
        if(request->to >= 0)
            len = MIN(len, request->to - request->from);
    }

    connection->offset = request->from;
    httpSetTimeout(connection, clientTimeout);
    do_log(D_CLIENT_DATA, "Serving on 0x%lx for 0x%lx: offset %d len %d\n",
           (unsigned long)connection, (unsigned long)object,
           connection->offset, len);
    do_stream_h(IO_WRITE |
                (connection->te == TE_CHUNKED && len > 0 ? IO_CHUNKED : 0),
                connection->fd, 0, 
                connection->buf, n,
                object->chunks[i].data + j, len,
                httpServeObjectStreamHandler, connection);
    return 1;

 fail:
    rc = 0;
    connection->len = 0;
    if(!(connection->flags & CONN_BIGBUF))
        rc = httpConnectionBigify(connection);
    if(rc > 0) {
        bufsize = bigBufferSize;
        goto again;
    }
    unlockChunk(object, i);
    return httpClientRawError(connection, 500,
                              rc == 0 ?
                              internAtom("No space for headers") :
                              internAtom("Couldn't allocate big buffer"), 0);
}