int
httpClientHandlerHeaders(FdEventHandlerPtr event, StreamRequestPtr srequest,
                         HTTPConnectionPtr connection)
{
    HTTPRequestPtr request;
    int rc;
    int method, version;
    AtomPtr url = NULL;
    int start;
    int code;
    AtomPtr message;

    start = 0;
    /* Work around clients working around NCSA lossage. */
    if(connection->reqbuf[0] == '\n')
        start = 1;
    else if(connection->reqbuf[0] == '\r' && connection->reqbuf[1] == '\n')
        start = 2;

    httpSetTimeout(connection, -1);
    rc = httpParseClientFirstLine(connection->reqbuf, start,
                                  &method, &url, &version);
    if(rc <= 0) {
        do_log(L_ERROR, "Couldn't parse client's request line\n");
        code = 400;
        message =  internAtom("Error in request line");
        goto fail;
    }

    do_log(D_CLIENT_REQ, "Client request: ");
    do_log_n(D_CLIENT_REQ, connection->reqbuf, rc - 1);
    do_log(D_CLIENT_REQ, "\n");

    if(version != HTTP_10 && version != HTTP_11) {
        do_log(L_ERROR, "Unknown client HTTP version\n");
        code = 400;
        message = internAtom("Error in first request line");
        goto fail;
    }

    if(method == METHOD_UNKNOWN) {
        code = 501;
        message =  internAtom("Method not implemented");
        goto fail;
    }

    request = httpMakeRequest();
    if(request == NULL) {
        do_log(L_ERROR, "Couldn't allocate client request.\n");
        code = 500;
        message = internAtom("Couldn't allocate client request");
        goto fail;
    }

    if(connection->version != HTTP_UNKNOWN && version != connection->version) {
        do_log(L_WARN, "Client version changed!\n");
    }

    connection->version = version;
    request->flags = REQUEST_PERSISTENT;
    request->method = method;
    request->cache_control = no_cache_control;
    httpQueueRequest(connection, request);
    connection->reqbegin = rc;
    return httpClientRequest(request, url);

 fail:
    if(url) releaseAtom(url);
    shutdown(connection->fd, 0);
    connection->reqlen = 0;
    connection->reqbegin = 0;
    httpConnectionDestroyReqbuf(connection);
    connection->flags &= ~CONN_READER;
    httpClientNewError(connection, METHOD_UNKNOWN, 0, code, message);
    return 1;

}