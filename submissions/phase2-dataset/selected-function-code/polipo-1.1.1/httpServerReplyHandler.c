int
httpServerReplyHandler(int status,
                       FdEventHandlerPtr event, 
                       StreamRequestPtr srequest)
{
    HTTPConnectionPtr connection = srequest->data;
    HTTPRequestPtr request = connection->request;
    int i, body;
    int bufsize = 
        (connection->flags & CONN_BIGBUF) ? bigBufferSize : CHUNK_SIZE;

    assert(request->object->flags & OBJECT_INPROGRESS);
    if(status < 0) {
        if(connection->serviced >= 1) {
            httpServerRestart(connection);
            return 1;
        }
        if(status != -ECLIENTRESET)
            do_log_error(L_ERROR, -status, "Read from server failed");
        httpServerAbort(connection, status != -ECLIENTRESET, 502, 
                        internAtomError(-status, "Read from server failed"));
        return 1;
    }

    i = findEndOfHeaders(connection->buf, 0, srequest->offset, &body);
    connection->len = srequest->offset;

    if(i >= 0) {
        request->time1 = current_time;
        return httpServerHandlerHeaders(status, event, srequest, connection);
    }

    if(status) {
        if(connection->serviced >= 1) {
            httpServerRestart(connection);
            return 1;
        }
        if(status < 0) {
            do_log(L_ERROR, 
                   "Error reading server headers: %d\n", -status);
            httpServerAbort(connection, status != -ECLIENTRESET, 502, 
                            internAtomError(-status, 
                                            "Error reading server headers"));
        } else
            httpServerAbort(connection, 1, 502, 
                            internAtom("Server dropped connection"));
        return 1;
    }

    if(connection->len >= bufsize) {
        int rc = 0;
        if(!(connection->flags & CONN_BIGBUF))
            rc = httpConnectionBigify(connection);
        if(rc == 0) {
            do_log(L_ERROR, "Couldn't find end of server's headers.\n");
            httpServerAbort(connection, 1, 502,
                            internAtom("Couldn't find end "
                                       "of server's headers"));
            return 1;
        } else if(rc < 0) {
            do_log(L_ERROR, "Couldn't allocate big buffer.\n");
            httpServerAbort(connection, 1, 500,
                            internAtom("Couldn't allocate big buffer"));
            return 1;
        }
        /* Can't just return 0 -- buf has moved. */
        do_stream(IO_READ,
                  connection->fd, connection->len,
                  connection->buf, bigBufferSize,
                  httpServerReplyHandler, connection);
        return 1;
    }

    return 0;
}