int 
httpClientSideHandler(int status,
                      FdEventHandlerPtr event,
                      StreamRequestPtr srequest)
{
    HTTPConnectionPtr connection = srequest->data;
    HTTPRequestPtr request = connection->request;
    HTTPRequestPtr requestee;
    HTTPConnectionPtr server;
    int push;
    int code;
    AtomPtr message = NULL;

    assert(connection->flags & CONN_SIDE_READER);

    if((request->object->flags & OBJECT_ABORTED) || 
       !(request->object->flags & OBJECT_INPROGRESS)) {
        code = request->object->code;
        message = retainAtom(request->object->message);
        goto fail;
    }
        
    if(status < 0) {
        do_log_error(L_ERROR, -status, "Reading from client");
        code = 502;
        message = internAtomError(-status, "Couldn't read from client");
        goto fail;
    }

    requestee = request->request;
    server = requestee->connection;

    push = MIN(srequest->offset - connection->reqlen, 
               connection->bodylen - connection->reqoffset);
    if(push > 0) {
        connection->reqlen += push;
        httpServerDoSide(server);
        return 1;
    }

    if(server->reqoffset >= connection->bodylen) {
        connection->flags &= ~(CONN_READER | CONN_SIDE_READER);
        return 1;
    }

    assert(status);
    do_log(L_ERROR, "Incomplete client request.\n");
    code = 502;
    message = internAtom("Incomplete client request");

 fail:
    request->error_code = code;
    if(request->error_message)
        releaseAtom(request->error_message);
    request->error_message = message;
    if(request->error_headers)
        releaseAtom(request->error_headers);
    request->error_headers = NULL;

    if(request->request) {
        shutdown(request->request->connection->fd, 2);
        pokeFdEvent(request->request->connection->fd, -ESHUTDOWN, POLLOUT);
    }
    notifyObject(request->object);
    connection->flags &= ~CONN_SIDE_READER;
    httpClientDiscardBody(connection);
    return 1;
}