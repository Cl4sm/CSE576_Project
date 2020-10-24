HTTPConnectionPtr
httpMakeConnection()
{
    HTTPConnectionPtr connection;
    connection = malloc(sizeof(HTTPConnectionRec));
    if(connection == NULL)
        return NULL;
    connection->flags = 0;
    connection->fd = -1;
    connection->buf = NULL;
    connection->len = 0;
    connection->offset = 0;
    connection->request = NULL;
    connection->request_last = NULL;
    connection->serviced = 0;
    connection->version = HTTP_UNKNOWN;
    connection->time = current_time.tv_sec;
    connection->timeout = NULL;
    connection->te = TE_IDENTITY;
    connection->reqbuf = NULL;
    connection->reqlen = 0;
    connection->reqbegin = 0;
    connection->reqoffset = 0;
    connection->bodylen = -1;
    connection->reqte = TE_IDENTITY;
    connection->chunk_remaining = 0;
    connection->server = NULL;
    connection->pipelined = 0;
    connection->connecting = 0;
    connection->server = NULL;
    return connection;
}