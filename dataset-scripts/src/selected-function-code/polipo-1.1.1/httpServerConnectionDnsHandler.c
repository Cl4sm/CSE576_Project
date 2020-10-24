int
httpServerConnectionDnsHandler(int status, GethostbynameRequestPtr request)
{
    HTTPConnectionPtr connection = request->data;

    httpSetTimeout(connection, -1);

    if(status <= 0) {
        AtomPtr message;
        message = internAtomF("Host %s lookup failed: %s",
                              request->name ?
                              request->name->string : "(unknown)",
                              request->error_message ?
                              request->error_message->string :
                              pstrerror(-status));
        do_log(L_ERROR, "Host %s lookup failed: %s (%d).\n", 
               request->name ?
               scrub(request->name->string) : "(unknown)",
               request->error_message ?
               request->error_message->string :
               pstrerror(-status), -status);
        connection->connecting = 0;
        if(connection->server->request)
            httpServerAbortRequest(connection->server->request, 1, 504,
                                   retainAtom(message));
        httpServerAbort(connection, 1, 502, message);
        return 1;
    }

    if(request->addr->string[0] == DNS_CNAME) {
        if(request->count > 10) {
            AtomPtr message = internAtom("DNS CNAME loop");
            do_log(L_ERROR, "DNS CNAME loop.\n");
            connection->connecting = 0;
            if(connection->server->request)
                httpServerAbortRequest(connection->server->request, 1, 504,
                                       retainAtom(message));
            httpServerAbort(connection, 1, 504, message);
            return 1;
        }
            
        httpSetTimeout(connection, serverTimeout);
        do_gethostbyname(request->addr->string + 1, request->count + 1,
                         httpServerConnectionDnsHandler,
                         connection);
        return 1;
    }

    connection->connecting = CONNECTING_CONNECT;
    httpSetTimeout(connection, serverTimeout);
    do_connect(retainAtom(request->addr), connection->server->addrindex,
               connection->server->port,
               httpServerConnectionHandler, connection);
    return 1;
}