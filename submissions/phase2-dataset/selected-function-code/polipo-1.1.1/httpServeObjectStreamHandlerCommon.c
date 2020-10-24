static int
httpServeObjectStreamHandlerCommon(int kind, int status,
                                   FdEventHandlerPtr event,
                                   StreamRequestPtr srequest)
{
    HTTPConnectionPtr connection = srequest->data;
    HTTPRequestPtr request = connection->request;
    int condition_result = httpCondition(request->object, request->condition);
    int i = connection->offset / CHUNK_SIZE;

    assert(!request->chandler);

    if(status == 0 && !streamRequestDone(srequest)) {
        httpSetTimeout(connection, clientTimeout);
        return 0;
    }

    httpSetTimeout(connection, -1);

    unlockChunk(request->object, i);
    if(kind == 2)
        unlockChunk(request->object, i + 1);

    if(status) {
        if(status < 0) {
            do_log_error(status == -ECONNRESET ? D_IO : L_ERROR, 
                         -status, "Couldn't write to client");
            if(status == -EIO || status == -ESHUTDOWN)
                httpClientFinish(connection, 2);
            else
                httpClientFinish(connection, 1);
        } else {
            do_log(D_IO, "Couldn't write to client: short write.\n");
            httpClientFinish(connection, 2);
        }
        return 1;
    }

    if(srequest->operation & IO_CHUNKED) {
        assert(srequest->offset > 2);
        connection->offset += srequest->offset - 2;
    } else {
        connection->offset += srequest->offset;
    }
    request->flags &= ~REQUEST_REQUESTED;

    if(request->object->flags & OBJECT_ABORTED) {
        httpClientFinish(connection, 1);
        return 1;
    }

    if(connection->request->method == METHOD_HEAD ||
       condition_result == CONDITION_NOT_MODIFIED) {
        httpClientFinish(connection, 0);
        return 1;
    }

    if(srequest->operation & IO_END)
        httpClientFinish(connection, 0);
    else {
        httpConnectionDestroyBuf(connection);
        lockChunk(connection->request->object,
                  connection->offset / CHUNK_SIZE);
        httpServeChunk(connection);
    }
    return 1;
}