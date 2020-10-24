int 
httpServerDoSide(HTTPConnectionPtr connection)
{
    HTTPRequestPtr request = connection->request;
    HTTPRequestPtr requestor = request->request;
    HTTPConnectionPtr client = requestor->connection;
    int len = MIN(client->reqlen - client->reqbegin,
                  connection->bodylen - connection->reqoffset);
    int doflush = 
        len > 0 &&
        (len >= maxSideBuffering ||
         client->reqbegin > 0 ||
         (connection->reqoffset + client->reqlen - client->reqbegin) >=
         connection->bodylen);
    int done = connection->reqoffset >= connection->bodylen;

    assert(connection->bodylen >= 0);

    httpSetTimeout(connection, 60);

    if(connection->reqlen > 0) {
        /* Send the headers, but don't send any part of the body if
           we're in wait_continue. */
        do_stream_2(IO_WRITE,
                    connection->fd, 0,
                    connection->reqbuf, connection->reqlen,
                    client->reqbuf + client->reqbegin, 
                    (request->flags & REQUEST_WAIT_CONTINUE) ? 0 : len,
                    httpServerSideHandler2, connection);
        httpServerReply(connection, 0);
    } else if(request->object->flags & OBJECT_ABORTED) {
        if(connection->reqbuf)
            dispose_chunk(connection->reqbuf);
        connection->reqbuf = NULL;
        connection->reqlen = 0;
        pokeFdEvent(connection->fd, -ESHUTDOWN, POLLIN);
        if(client->flags & CONN_READER) {
            client->flags |= CONN_SIDE_READER;
            do_stream(IO_READ | IO_IMMEDIATE | IO_NOTNOW,
                      client->fd, 0, NULL, 0,
                      httpClientSideHandler, client);
        }
    } else if(!(request->flags & REQUEST_WAIT_CONTINUE) && doflush) {
        /* Make sure there's a reqbuf, as httpServerFinish uses
           it to determine if there's a writer. */
        if(connection->reqbuf == NULL)
            connection->reqbuf = get_chunk();
        assert(connection->reqbuf != NULL);
        do_stream(IO_WRITE,
                  connection->fd, 0,
                  client->reqbuf + client->reqbegin, len,
                  httpServerSideHandler, connection);
    } else {
        if(connection->reqbuf) {
            httpConnectionDestroyReqbuf(connection);
            connection->reqlen = 0;
        }
        if(request->flags & REQUEST_WAIT_CONTINUE) {
            ConditionHandlerPtr chandler;
            do_log(D_SERVER_CONN, "W... %s:%d.\n",
                   scrub(connection->server->name), connection->server->port);
            chandler = 
                conditionWait(&request->object->condition,
                              httpServerContinueConditionHandler,
                              sizeof(connection), &connection);
            if(chandler)
                return 1;
            else
                do_log(L_ERROR, "Couldn't register condition handler.\n");
            /* Fall through -- the client side will clean up. */
        }
        client->flags |= CONN_SIDE_READER;
        do_stream(IO_READ | (done ? IO_IMMEDIATE : 0 ) | IO_NOTNOW,
                  client->fd, client->reqlen,
                  client->reqbuf, CHUNK_SIZE,
                  httpClientSideHandler, client);
    }
    return 1;
}