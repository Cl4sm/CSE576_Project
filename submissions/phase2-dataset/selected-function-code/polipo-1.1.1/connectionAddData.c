int
connectionAddData(HTTPConnectionPtr connection, int skip)
{
    HTTPRequestPtr request = connection->request;
    ObjectPtr object = request->object;
    int rc;

    if(connection->te == TE_IDENTITY) {
        int len;
        
        len = connection->len - skip;
        if(object->length >= 0) {
            len = MIN(object->length - connection->offset, len);
        }
        if(request->to >= 0)
            len = MIN(request->to - connection->offset, len);
        if(len > 0) {
            rc = objectAddData(object, connection->buf + skip,
                               connection->offset, len);
            if(rc < 0)
                return -1;
            connection->offset += len;
            connection->len -= (len + skip);
            do_log(D_SERVER_OFFSET, "0x%lx(0x%lx): offset = %d\n",
                   (unsigned long)connection, (unsigned long)object,
                   connection->offset);
        }

        if(connection->len > 0 && skip + len > 0) {
            memmove(connection->buf,
                    connection->buf + skip + len, connection->len);
        }

        if((object->length >= 0 && object->length <= connection->offset) ||
           (request->to >= 0 && request->to <= connection->offset)) {
            notifyObject(object);
            return 1;
        } else {
            if(len > 0)
                notifyObject(object);
            return 0;
        }
    } else if(connection->te == TE_CHUNKED) {
        int i = skip, j, size;
        /* connection->chunk_remaining is 0 at the end of a chunk, -1
           after the CR/LF pair ending a chunk, and -2 after we've
           seen a chunk of length 0. */
        if(connection->chunk_remaining > -2) {
            while(1) {
                if(connection->chunk_remaining <= 0) {
                    if(connection->chunk_remaining == 0) {
                        if(connection->len < i + 2)
                            break;
                        if(connection->buf[i] != '\r' ||
                           connection->buf[i + 1] != '\n')
                            return -1;
                        i += 2;
                        connection->chunk_remaining = -1;
                    }
                    if(connection->len < i + 2)
                        break;
                    j = parseChunkSize(connection->buf, i,
                                       connection->len, &size);
                    if(j < 0)
                        return -2;
                    if(j == 0)
                        break;
                    else
                        i = j;
                    if(size == 0) {
                        connection->chunk_remaining = -2;
                        break;
                    } else {
                        connection->chunk_remaining = size;
                    }
                } else {
                    /* connection->chunk_remaining > 0 */
                    size = MIN(connection->chunk_remaining,
                               connection->len - i);
                    if(size <= 0)
                        break;
                    rc = objectAddData(object, connection->buf + i,
                                       connection->offset, size);
                    connection->offset += size;
                    if(rc < 0)
                        return -1;
                    i += size;
                    connection->chunk_remaining -= size;
                    do_log(D_SERVER_OFFSET, "0x%lx(0x%lx): offset = %d\n",
                           (unsigned long)connection, 
                           (unsigned long)object,
                           connection->offset);
                }
            }
        }
        connection->len -= i;
        if(connection->len > 0)
            memmove(connection->buf, connection->buf + i, connection->len);
        if(i > 0 || connection->chunk_remaining == -2)
            notifyObject(object);
        if(connection->chunk_remaining == -2)
            return 1;
        else
            return 0;
    } else {
        abort();
    }
}