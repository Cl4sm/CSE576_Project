int
httpClientHandler(int status,
                  FdEventHandlerPtr event, StreamRequestPtr request)
{
    HTTPConnectionPtr connection = request->data;
    int i, body;
    int bufsize = 
        (connection->flags & CONN_BIGREQBUF) ? bigBufferSize : CHUNK_SIZE;

    assert(connection->flags & CONN_READER);

    /* There's no point trying to do something with this request if
       the client has shut the connection down -- HTTP doesn't do
       half-open connections. */
    if(status != 0) {
        connection->reqlen = 0;
        httpConnectionDestroyReqbuf(connection);
        if(!(connection->flags & CONN_WRITER)) {
            connection->flags &= ~CONN_READER;
            if(status > 0 || status == -ECONNRESET || status == -EDOSHUTDOWN)
                httpClientFinish(connection, 2);
            else
                httpClientFinish(connection, 1);
            return 1;
        }
        httpClientAbort(connection, status > 0 || status == -ECONNRESET);
        connection->flags &= ~CONN_READER;
        return 1;
    }

    i = findEndOfHeaders(connection->reqbuf, 0, request->offset, &body);
    connection->reqlen = request->offset;

    if(i >= 0) {
        connection->reqbegin = i;
        httpClientHandlerHeaders(event, request, connection);
        return 1;
    }

    if(connection->reqlen >= bufsize) {
        int rc = 0;
        if(!(connection->flags & CONN_BIGREQBUF))
            rc = httpConnectionBigifyReqbuf(connection);
        if((connection->flags & CONN_BIGREQBUF) &&
           connection->reqlen < bigBufferSize) {
            do_stream(IO_READ, connection->fd, connection->reqlen,
                      connection->reqbuf, bigBufferSize,
                      httpClientHandler, connection);
            return 1;
        }
        connection->reqlen = 0;
        httpConnectionDestroyReqbuf(connection);
        if(rc < 0) {
            do_log(L_ERROR, "Couldn't allocate big buffer.\n");
            httpClientNewError(connection, METHOD_UNKNOWN, 0, 400, 
                               internAtom("Couldn't allocate big buffer"));
        } else {
            do_log(L_ERROR, "Couldn't find end of client's headers.\n");
            httpClientNewError(connection, METHOD_UNKNOWN, 0, 400, 
                               internAtom("Couldn't find end of headers"));
        }
        return 1;
    }
    httpSetTimeout(connection, clientTimeout);
    return 0;
}