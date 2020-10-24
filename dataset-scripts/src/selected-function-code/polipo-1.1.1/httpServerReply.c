void
httpServerReply(HTTPConnectionPtr connection, int immediate)
{
    assert(connection->pipelined > 0);

    if(connection->request->request == NULL) {
        do_log(L_WARN, "Aborting pipeline on %s:%d.\n",
               scrub(connection->server->name), connection->server->port);
        httpServerFinish(connection, 1, 0);
        return;
    }

    do_log(D_SERVER_CONN, "R: %s (%d)\n",
           scrub(connection->request->object->key),
           connection->request->method);

    if(connection->len == 0)
        httpConnectionDestroyBuf(connection);

    httpSetTimeout(connection, serverTimeout);
    do_stream_buf(IO_READ | (immediate ? IO_IMMEDIATE : 0) | IO_NOTNOW,
                  connection->fd, connection->len,
                  &connection->buf, CHUNK_SIZE,
                  httpServerReplyHandler, connection);
}