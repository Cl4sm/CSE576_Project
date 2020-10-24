int
httpServeChunk(HTTPConnectionPtr connection)
{
    HTTPRequestPtr request = connection->request;
    ObjectPtr object = request->object;
    int i = connection->offset / CHUNK_SIZE;
    int j = connection->offset - (i * CHUNK_SIZE);
    int to, len, len2, end;
    int rc;

    /* This must be called with chunk i locked. */
    assert(object->chunks[i].locked > 0);

    if(object->flags & OBJECT_ABORTED)
        goto fail;

    if(object->length >= 0 && request->to >= 0)
        to = MIN(request->to, object->length);
    else if(object->length >= 0)
        to = object->length;
    else if(request->to >= 0)
        to = request->to;
    else
        to = -1;

    len = 0;
    if(i < object->numchunks)
        len = object->chunks[i].size - j;

    if(request->method != METHOD_HEAD && 
       len < CHUNK_SIZE && connection->offset + len < to) {
        objectFillFromDisk(object, connection->offset + len, 2);
        len = object->chunks[i].size - j;
    }

    if(to >= 0)
        len = MIN(len, to - connection->offset);

    if(len <= 0) {
        if(to >= 0 && connection->offset >= to) {
            if(request->chandler) {
                unregisterConditionHandler(request->chandler);
                request->chandler = NULL;
            }
            unlockChunk(object, i);
            if(connection->te == TE_CHUNKED) {
                httpSetTimeout(connection, clientTimeout);
                do_stream(IO_WRITE | IO_CHUNKED | IO_END,
                          connection->fd, 0, NULL, 0,
                          httpServeObjectFinishHandler, connection);
            } else {
                httpClientFinish(connection,
                                 !(object->length >= 0 &&
                                   connection->offset >= object->length));
            }
            return 1;
        } else {
            if(!request->chandler) {
                request->chandler =
                    conditionWait(&object->condition, 
                                  httpServeObjectHandler,
                                  sizeof(connection), &connection);
                if(!request->chandler) {
                    do_log(L_ERROR, "Couldn't register condition handler\n");
                    goto fail;
                }
            }
            if(!(object->flags & OBJECT_INPROGRESS)) {
                if(object->flags & OBJECT_SUPERSEDED) {
                    goto fail;
                }
                if(REQUEST_SIDE(request)) goto fail;
                rc = object->request(object, request->method,
                                     connection->offset, -1, request,
                                     object->request_closure);
                if(rc <= 0) goto fail;
            }
            return 1;
        }
    } else {
        /* len > 0 */
        if(request->method != METHOD_HEAD)
            objectFillFromDisk(object, (i + 1) * CHUNK_SIZE, 1);
        if(request->chandler) {
            unregisterConditionHandler(request->chandler);
            request->chandler = NULL;
        }
        len2 = 0;
        if(j + len == CHUNK_SIZE && object->numchunks > i + 1) {
            len2 = object->chunks[i + 1].size;
            if(to >= 0)
                len2 = MIN(len2, to - (i + 1) * CHUNK_SIZE);
        }
        /* Lock early -- httpServerRequest may get_chunk */
        if(len2 > 0)
            lockChunk(object, i + 1);
        if(object->length >= 0 && 
           connection->offset + len + len2 == object->length)
            end = 1;
        else
            end = 0;
        /* Prefetch */
        if(!(object->flags & OBJECT_INPROGRESS) && !REQUEST_SIDE(request)) {
            if(object->chunks[i].size < CHUNK_SIZE &&
               to >= 0 && connection->offset + len + 1 < to)
                object->request(object, request->method,
                                connection->offset + len, -1, request,
                                object->request_closure);
            else if(i + 1 < object->numchunks &&
                    object->chunks[i + 1].size == 0 &&
                    to >= 0 && (i + 1) * CHUNK_SIZE + 1 < to)
                object->request(object, request->method,
                                (i + 1) * CHUNK_SIZE, -1, request,
                                object->request_closure);
        }
        if(len2 == 0) {
            httpSetTimeout(connection, clientTimeout);
            do_log(D_CLIENT_DATA, 
                   "Serving on 0x%lx for 0x%lx: offset %d len %d\n",
                   (unsigned long)connection, (unsigned long)object,
                   connection->offset, len);
            /* IO_NOTNOW in order to give other clients a chance to run. */
            do_stream(IO_WRITE | IO_NOTNOW |
                      (connection->te == TE_CHUNKED ? IO_CHUNKED : 0) |
                      (end ? IO_END : 0),
                      connection->fd, 0, 
                      object->chunks[i].data + j, len,
                      httpServeObjectStreamHandler, connection);
        } else {
            httpSetTimeout(connection, clientTimeout);
            do_log(D_CLIENT_DATA, 
                   "Serving on 0x%lx for 0x%lx: offset %d len %d + %d\n",
                   (unsigned long)connection, (unsigned long)object,
                   connection->offset, len, len2);
            do_stream_2(IO_WRITE | IO_NOTNOW |
                        (connection->te == TE_CHUNKED ? IO_CHUNKED : 0) |
                        (end ? IO_END : 0),
                        connection->fd, 0, 
                        object->chunks[i].data + j, len,
                        object->chunks[i + 1].data, len2,
                        httpServeObjectStreamHandler2, connection);
        }            
        return 1;
    }

    abort();

 fail:
    unlockChunk(object, i);
    if(request->chandler)
        unregisterConditionHandler(request->chandler);
    request->chandler = NULL;
    httpClientFinish(connection, 1);
    return 1;
}