int
httpServerDirectHandlerCommon(int kind, int status,
                              FdEventHandlerPtr event, 
                              StreamRequestPtr srequest)
{
    HTTPConnectionPtr connection = srequest->data;
    HTTPRequestPtr request = connection->request;
    ObjectPtr object = request->object;
    int i = connection->offset / CHUNK_SIZE;
    int to, end, end1;

    assert(request->object->flags & OBJECT_INPROGRESS);

    httpSetTimeout(connection, -1);

    if(status < 0) {
        unlockChunk(object, i);
        if(kind == 2) unlockChunk(object, i + 1);
        if(status != -ECLIENTRESET)
            do_log_error(L_ERROR, -status, "Read from server failed");
        httpServerAbort(connection, status != -ECLIENTRESET, 502,
                        internAtomError(-status, "Read from server failed"));
        return 1;
    }

    /* We have incestuous knowledge of the decisions made in
       httpServerReadData */
    if(request->to >= 0)
        to = request->to;
    else
        to = object->length;
    if(connection->te == TE_CHUNKED)
        end = connection->offset + connection->chunk_remaining;
    else
        end = to;
    /* The amount of data actually read into the object */
    end1 = MIN(end, i * CHUNK_SIZE + MIN(kind * CHUNK_SIZE, srequest->offset));

    assert(end >= 0);
    assert(end1 >= i * CHUNK_SIZE);
    assert(end1 - 2 * CHUNK_SIZE <= i * CHUNK_SIZE);

    object->chunks[i].size = 
        MAX(object->chunks[i].size, MIN(end1 - i * CHUNK_SIZE, CHUNK_SIZE));
    if(kind == 2 && end1 > (i + 1) * CHUNK_SIZE) {
        object->chunks[i + 1].size =
            MAX(object->chunks[i + 1].size, end1 - (i + 1) * CHUNK_SIZE);
    }
    if(connection->te == TE_CHUNKED) {
        connection->chunk_remaining -= (end1 - connection->offset);
        assert(connection->chunk_remaining >= 0);
    }
    connection->offset = end1;
    object->size = MAX(object->size, end1);
    unlockChunk(object, i);
    if(kind == 2) unlockChunk(object, i + 1);

    if(i * CHUNK_SIZE + srequest->offset > end1) {
        connection->len = i * CHUNK_SIZE + srequest->offset - end1;
        return httpServerIndirectHandlerCommon(connection, status);
    } else {
        notifyObject(object);
        if(status) {
            if(connection->te == TE_CHUNKED ||
               (end >= 0 && connection->offset < end)) {
                do_log(L_ERROR, "Server dropped connection.\n");
                httpServerAbort(connection, 1, 502, 
                                internAtom("Server dropped connection"));
            } else
                httpServerFinish(connection, 1, 0);
            return 1;
        } else {
            return httpServerReadData(connection, 0);
        }
    }
}