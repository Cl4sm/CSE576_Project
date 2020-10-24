int
httpClientNoticeRequest(HTTPRequestPtr request, int novalidate)
{
    HTTPConnectionPtr connection = request->connection;
    ObjectPtr object = request->object;
    int serveNow = (request == connection->request);
    int validate = 0;
    int conditional = 0;
    int local, haveData;
    int rc;

    assert(!request->chandler);

    if(request->error_code) {
        if((request->flags & REQUEST_FORCE_ERROR) || REQUEST_SIDE(request) ||
           request->object == NULL ||
           (request->object->flags & OBJECT_LOCAL) ||
           (request->object->flags & OBJECT_ABORTED) ||
           (relaxTransparency < 1 && !proxyOffline)) {
            if(serveNow) {
                connection->flags |= CONN_WRITER;
                return httpClientRawErrorHeaders(connection,
                                                 request->error_code, 
                                                 retainAtom(request->
                                                            error_message),
                                                 0, request->error_headers);
            } else {
                return 1;
            }
        }
    }

    if(REQUEST_SIDE(request)) {
        assert(!(request->flags & REQUEST_REQUESTED));
        if(serveNow) {
            assert(!request->chandler);
            request->chandler =
                conditionWait(&request->object->condition, 
                              httpClientGetHandler,
                              sizeof(request), &request);
            if(request->chandler == NULL) {
                do_log(L_ERROR, "Couldn't register condition handler.\n");
                connection->flags |= CONN_WRITER;
                httpClientRawError(connection, 500,
                                   internAtom("Couldn't register "
                                              "condition handler"),
                                   0);
                return 1;
            }
            connection->flags |= CONN_WRITER;
            rc = object->request(request->object,
                                 request->method,
                                 request->from, request->to, 
                                 request,
                                 request->object->request_closure);
        }
        return 1;
    }

    local = urlIsLocal(object->key, object->key_size);
    objectFillFromDisk(object, request->from,
                       request->method == METHOD_HEAD ? 0 : 1);

    /* The spec doesn't strictly forbid 206 for non-200 instances, but doing
       that breaks some client software. */
    if(object->code && object->code != 200) {
        request->from = 0;
        request->to = -1;
    }

    if(request->condition && request->condition->ifrange) {
        if(!object->etag || 
           strcmp(object->etag, request->condition->ifrange) != 0) {
            request->from = 0;
            request->to = -1;
        }
    }

    if(object->flags & OBJECT_DYNAMIC) {
        request->from = 0;
        request->to = -1;
    }

    if(request->method == METHOD_HEAD)
        haveData = !(request->object->flags & OBJECT_INITIAL);
    else
        haveData = 
            (request->object->length >= 0 && 
             request->object->length <= request->from) ||
            (objectHoleSize(request->object, request->from) == 0);

    if(request->flags & REQUEST_REQUESTED)
        validate = 0;
    else if(novalidate || (!local && proxyOffline))
        validate = 0;
    else if(local)
        validate = 
            objectMustRevalidate(request->object, &request->cache_control);
    else if(request->cache_control.flags & CACHE_ONLY_IF_CACHED)
        validate = 0;
    else if((request->object->flags & OBJECT_FAILED) &&
            !(object->flags & OBJECT_INPROGRESS) &&
            !relaxTransparency)
        validate = 1;
    else if(request->method != METHOD_HEAD &&
            !objectHasData(object, request->from, request->to) &&
            !(object->flags & OBJECT_INPROGRESS))
        validate = 1;
    else if(objectMustRevalidate((relaxTransparency <= 1 ? 
                                  request->object : NULL),
                                 &request->cache_control))
        validate = 1;
    else
        validate = 0;

    if(request->cache_control.flags & CACHE_ONLY_IF_CACHED) {
        validate = 0;
        if(!haveData) {
            if(serveNow) {
                connection->flags |= CONN_WRITER;
                return httpClientRawError(connection, 504,
                                          internAtom("Object not in cache"),
                                          0);
            } else
                return 1;
        }
    }

    if(!(request->object->flags & OBJECT_VALIDATING) &&
       ((!validate && haveData) ||
        (request->object->flags & OBJECT_FAILED))) {
        if(serveNow) {
            connection->flags |= CONN_WRITER;
            lockChunk(request->object, request->from / CHUNK_SIZE);
            return httpServeObject(connection);
        } else {
            return 1;
        }
    }

    if((request->flags & REQUEST_REQUESTED) &&
       !(request->object->flags & OBJECT_INPROGRESS)) {
        /* This can happen either because the server side ran out of
           memory, or because it is using HEAD validation.  We mark
           the object to be fetched again. */
        request->flags &= ~REQUEST_REQUESTED;
    }

    if(serveNow) {
        connection->flags |= CONN_WRITER;
        if(!local && proxyOffline)
            return httpClientRawError(connection, 502, 
                                      internAtom("Disconnected operation "
                                                 "and object not in cache"),
                                      0);
        request->chandler =
            conditionWait(&request->object->condition, httpClientGetHandler, 
                          sizeof(request), &request);
        if(request->chandler == NULL) {
            do_log(L_ERROR, "Couldn't register condition handler.\n");
            return httpClientRawError(connection, 503,
                                      internAtom("Couldn't register "
                                                 "condition handler"), 0);
        }
    }

    if(request->object->flags & OBJECT_VALIDATING)
        return 1;

    conditional = (haveData && request->method == METHOD_GET);
    if(!mindlesslyCacheVary && (request->object->cache_control & CACHE_VARY))
        conditional = conditional && (request->object->etag != NULL);

    conditional =
        conditional && !(request->object->cache_control & CACHE_MISMATCH);

    if(!(request->object->flags & OBJECT_INPROGRESS))
        request->object->flags |= OBJECT_VALIDATING;
    rc = request->object->request(request->object,
                                  conditional ? METHOD_CONDITIONAL_GET : 
                                  request->method,
                                  request->from, request->to, request,
                                  request->object->request_closure);
    if(rc < 0) {
        if(request->chandler)
            unregisterConditionHandler(request->chandler);
        request->chandler = NULL;
        request->object->flags &= ~OBJECT_VALIDATING;
        request->object->flags |= OBJECT_FAILED;
        if(request->error_message)
            releaseAtom(request->error_message);
        request->error_code = 503;
        request->error_message = internAtom("Couldn't schedule get");
    }
    return 1;
}