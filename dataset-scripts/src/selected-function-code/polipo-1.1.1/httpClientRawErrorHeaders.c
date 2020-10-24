int
httpClientRawErrorHeaders(HTTPConnectionPtr connection,
                          int code, AtomPtr message,
                          int close, AtomPtr headers)
{
    int fd = connection->fd;
    int n;
    char *url; int url_len;
    char *etag;

    assert(connection->flags & CONN_WRITER);
    assert(code != 0);

    if(close >= 0) {
        if(connection->request)
            close = 
                close || !(connection->request->flags & REQUEST_PERSISTENT);
        else
            close = 1;
    }
    if(connection->request && connection->request->object) {
        url = connection->request->object->key;
        url_len = connection->request->object->key_size;
        etag = connection->request->object->etag;
    } else {
        url = NULL;
        url_len = 0;
        etag = NULL;
    }

    if(connection->buf == NULL) {
        connection->buf = get_chunk();
        if(connection->buf == NULL) {
            httpClientFinish(connection, 1);
            return 1;
        }
    }

    n = httpWriteErrorHeaders(connection->buf, CHUNK_SIZE, 0,
                              connection->request &&
                              connection->request->method != METHOD_HEAD,
                              code, message, close > 0, headers,
                              url, url_len, etag);
    if(n <= 0) {
        shutdown(connection->fd, 1);
        if(close >= 0)
            httpClientFinish(connection, 1);
        return 1;
    }

    httpSetTimeout(connection, clientTimeout);
    do_stream(IO_WRITE, fd, 0, connection->buf, n, 
              close > 0 ? httpErrorStreamHandler :
              close == 0 ? httpErrorNocloseStreamHandler :
              httpErrorNofinishStreamHandler,
              connection);

    return 1;
}