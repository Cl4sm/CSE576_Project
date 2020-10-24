static int
httpClientShutdownHandler(int status,
                          FdEventHandlerPtr event, StreamRequestPtr request)
{
    HTTPConnectionPtr connection = request->data;

    assert(connection->flags & CONN_READER);

    if(!(connection->flags & CONN_WRITER)) {
        connection->flags &= ~CONN_READER;
        connection->reqlen = 0;
        httpConnectionDestroyReqbuf(connection);
        if(status && status != -EDOGRACEFUL)
            httpClientFinish(connection, 2);
        else
            httpClientFinish(connection, 1);
        return 1;
    }

    httpClientDelayedShutdown(connection);
    return 1;
}