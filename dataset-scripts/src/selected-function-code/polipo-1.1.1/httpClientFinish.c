void
httpClientFinish(HTTPConnectionPtr connection, int s)
{
    HTTPRequestPtr request = connection->request;

    assert(!(request && request->request 
             && request->request->request != request));

    if(s == 0) {
        if(!request || !(request->flags & REQUEST_PERSISTENT))
            s = 1;
    }

    httpConnectionDestroyBuf(connection);

    connection->flags &= ~CONN_WRITER;

    if(connection->flags & CONN_SIDE_READER) {
        /* We're in POST or PUT and the reader isn't done yet.
           Wait for the read side to close the connection. */
        assert(request && (connection->flags & CONN_READER));
        if(s >= 2) {
            pokeFdEvent(connection->fd, -EDOSHUTDOWN, POLLIN);
        } else {
            pokeFdEvent(connection->fd, -EDOGRACEFUL, POLLIN);
        }
        return;
    }

    if(connection->timeout) 
        cancelTimeEvent(connection->timeout);
    connection->timeout = NULL;

    if(request) {
        HTTPRequestPtr requestee;

        requestee = request->request;
        if(requestee) {
            request->request = NULL;
            requestee->request = NULL;
        }
        if(requestee)
            httpServerClientReset(requestee);
        if(request->chandler) {
            request->error_code = 500;
            request->error_message = internAtom("Connection finishing");
            abortConditionHandler(request->chandler);
            request->chandler = NULL;
        }
            
        if(request->object) {
            if(request->object->requestor == request)
                request->object->requestor = NULL;
            releaseObject(request->object);
            request->object = NULL;
        }
        httpDequeueRequest(connection);
        httpDestroyRequest(request);
        request = NULL;

    }

    connection->len = -1;
    connection->offset = 0;
    connection->te = TE_IDENTITY;

    if(!s) {
        assert(connection->fd > 0);
        connection->serviced++;
        httpSetTimeout(connection, clientTimeout);
        if(!(connection->flags & CONN_READER)) {
            if(connection->reqlen == 0)
                httpConnectionDestroyReqbuf(connection);
            else if((connection->flags & CONN_BIGREQBUF) &&
                    connection->reqlen < CHUNK_SIZE)
                httpConnectionUnbigifyReqbuf(connection);
            connection->flags |= CONN_READER;
            httpSetTimeout(connection, clientTimeout);
            do_stream_buf(IO_READ | IO_NOTNOW |
                          (connection->reqlen ? IO_IMMEDIATE : 0),
                          connection->fd, connection->reqlen,
                          &connection->reqbuf,
                          (connection->flags & CONN_BIGREQBUF) ?
                          bigBufferSize : CHUNK_SIZE,
                          httpClientHandler, connection);
        }
        /* The request has already been validated when it first got
           into the queue */
        if(connection->request) {
            if(connection->request->object != NULL)
                httpClientNoticeRequest(connection->request, 1);
            else
                assert(connection->flags & CONN_READER);
        }
        return;
    }
    
    do_log(D_CLIENT_CONN, "Closing client connection 0x%lx\n",
           (unsigned long)connection);

    if(connection->flags & CONN_READER) {
        httpSetTimeout(connection, 10);
        if(connection->fd < 0) return;
        if(s >= 2) {
            pokeFdEvent(connection->fd, -EDOSHUTDOWN, POLLIN);
        } else {
            pokeFdEvent(connection->fd, -EDOGRACEFUL, POLLIN);
        }
        return;
    }
    while(1) {
        HTTPRequestPtr requestee;
        request = connection->request;
        if(!request)
            break;
        requestee = request->request;
        request->request = NULL;
        if(requestee) {
            requestee->request = NULL;
            httpServerClientReset(requestee);
        }
        if(request->chandler)
            abortConditionHandler(request->chandler);
        request->chandler = NULL;
        if(request->object && request->object->requestor == request)
            request->object->requestor = NULL;
        httpDequeueRequest(connection);
        httpDestroyRequest(request);
    }
    httpConnectionDestroyReqbuf(connection);
    if(connection->timeout)
        cancelTimeEvent(connection->timeout);
    connection->timeout = NULL;
    if(connection->fd >= 0) {
        if(s >= 2)
            CLOSE(connection->fd);
        else
            lingeringClose(connection->fd);
    }
    connection->fd = -1;
    free(connection);
}