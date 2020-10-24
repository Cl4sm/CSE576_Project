static int
httpServerSideHandlerCommon(int kind, int status,
                            FdEventHandlerPtr event,
                            StreamRequestPtr srequest)
{
    HTTPConnectionPtr connection = srequest->data;
    HTTPRequestPtr request = connection->request;
    HTTPRequestPtr requestor = request->request;
    HTTPConnectionPtr client = requestor->connection;
    int bodylen;

    assert(request->object->flags & OBJECT_INPROGRESS);

    if(status) {
        do_log_error(L_ERROR, -status, "Couldn't write to server");
        httpConnectionDestroyReqbuf(connection);
        if(status != -ECLIENTRESET)
            shutdown(connection->fd, 2);
        abortObject(request->object, 502,
                    internAtom("Couldn't write to server"));
        /* Let the read side handle the error */
        httpServerDoSide(connection);
        return 1;
    }

    assert(srequest->offset > 0);

    if(kind == 2) {
        if(srequest->offset < connection->reqlen)
            return 0;
        bodylen = srequest->offset - connection->reqlen;
        connection->reqlen = 0;
        httpConnectionDestroyReqbuf(connection);
    } else {
        bodylen = srequest->offset;
    }


    assert(client->reqbegin + bodylen <= client->reqlen);

    if(client->reqlen > client->reqbegin + bodylen)
        memmove(client->reqbuf, client->reqbuf + client->reqbegin + bodylen,
                client->reqlen - client->reqbegin - bodylen);
    client->reqlen -= bodylen + client->reqbegin;
    client->reqbegin = 0;
    connection->reqoffset += bodylen;
    httpServerDoSide(connection);
    return 1;
}