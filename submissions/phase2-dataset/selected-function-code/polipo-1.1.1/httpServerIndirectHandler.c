int
httpServerIndirectHandler(int status,
                          FdEventHandlerPtr event, 
                          StreamRequestPtr srequest)
{
    HTTPConnectionPtr connection = srequest->data;
    assert(connection->request->object->flags & OBJECT_INPROGRESS);

    httpSetTimeout(connection, -1);
    if(status < 0) {
        if(status != -ECLIENTRESET)
            do_log_error(L_ERROR, -status, "Read from server failed");
        httpServerAbort(connection, status != -ECLIENTRESET, 502,
                        internAtomError(-status, "Read from server failed"));
        return 1;
    }

    connection->len = srequest->offset;

    return httpServerIndirectHandlerCommon(connection, status);
}