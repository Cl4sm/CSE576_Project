int
httpServerReadData(HTTPConnectionPtr connection, int immediate)
{
    HTTPRequestPtr request = connection->request;
    ObjectPtr object = request->object;
    int to = -1;

    assert(object->flags & OBJECT_INPROGRESS);

    if(request->request == NULL) {
        httpServerFinish(connection, 1, 0);
        return 1;
    }

    if(request->to >= 0)
        to = request->to;
    else
        to = object->length;

    if(to >= 0 && to == connection->offset) {
        httpServerFinish(connection, 0, 0);
        return 1;
    }

    if(connection->len == 0 &&
       ((connection->te == TE_IDENTITY && to > connection->offset) ||
        (connection->te == TE_CHUNKED && connection->chunk_remaining > 0))) {
        /* Read directly into the object */
        int i = connection->offset / CHUNK_SIZE;
        int j = connection->offset % CHUNK_SIZE;
        int end, len, more;
        /* See httpServerDirectHandlerCommon if you change this */
        if(connection->te == TE_CHUNKED) {
            len = connection->chunk_remaining;
            /* The logic here is that we want more to just fit the
               chunk header if we're doing a large read, but do a
               large read if we would otherwise do a small one.  The
               magic constant 2000 comes from the assumption that the
               server uses chunks that have a size that are a power of
               two (possibly including the chunk header), and that we
               want a full ethernet packet to fit into our read. */
            more = (len >= 2000 ? 20 : MIN(2048 - len, CHUNK_SIZE));
        } else {
            len = to - connection->offset;
            /* We read more data only when there is a reasonable
               chance of there being another reply coming. */
            more = (connection->pipelined > 1) ? CHUNK_SIZE : 0;
        }
        end = len + connection->offset;

        httpConnectionDestroyBuf(connection);

        /* The order of allocation is important in case we run out of
           memory. */
        lockChunk(object, i);
        if(object->chunks[i].data == NULL)
            object->chunks[i].data = get_chunk();
        if(object->chunks[i].data && object->chunks[i].size >= j) {
            if(len + j > CHUNK_SIZE) {
                lockChunk(object, i + 1);
                if(object->chunks[i + 1].data == NULL)
                    object->chunks[i + 1].data = get_chunk();
                /* Unless we're grabbing all len of data, we do not
                   want to do an indirect read immediately afterwards. */
                if(more && len + j <= 2 * CHUNK_SIZE) {
                    if(!connection->buf)
                        connection->buf = get_chunk(); /* checked below */
                }
                if(object->chunks[i + 1].data) {
                    do_stream_3(IO_READ | IO_NOTNOW, connection->fd, j,
                                object->chunks[i].data, CHUNK_SIZE,
                                object->chunks[i + 1].data,
                                MIN(CHUNK_SIZE,
                                    end - (i + 1) * CHUNK_SIZE),
                                connection->buf, connection->buf ? more : 0,
                                httpServerDirectHandler2, connection);
                    return 1;
                }
                unlockChunk(object, i + 1);
            }
            if(more && len + j <= CHUNK_SIZE) {
                if(!connection->buf)
                    connection->buf = get_chunk();
            }
            do_stream_2(IO_READ | IO_NOTNOW, connection->fd, j,
                        object->chunks[i].data,
                        MIN(CHUNK_SIZE, end - i * CHUNK_SIZE),
                        connection->buf, connection->buf ? more : 0,
                        httpServerDirectHandler, connection);
            return 1;
        } else {
            unlockChunk(object, i);
        }
    }
       
    if(connection->len == 0)
        httpConnectionDestroyBuf(connection);

    httpSetTimeout(connection, serverTimeout);
    do_stream_buf(IO_READ | IO_NOTNOW |
                  ((immediate && connection->len) ? IO_IMMEDIATE : 0),
                  connection->fd, connection->len,
                  &connection->buf,
                  (connection->te == TE_CHUNKED ? 
                   MIN(2048, CHUNK_SIZE) : CHUNK_SIZE),
                  httpServerIndirectHandler, connection);
    return 1;
}