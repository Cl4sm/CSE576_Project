int
httpServerConnectionHandlerCommon(int status, HTTPConnectionPtr connection)
{
    httpSetTimeout(connection, -1);

    if(status < 0) {
        AtomPtr message = 
            internAtomError(-status, "Connect to %s:%d failed",
                            connection->server->name,
                            connection->server->port);
        if(status != -ECLIENTRESET)
            do_log_error(L_ERROR, -status, "Connect to %s:%d failed",
                         scrub(connection->server->name),
                         connection->server->port);
        connection->connecting = 0;
        if(connection->server->request)
            httpServerAbortRequest(connection->server->request,
                                   status != -ECLIENTRESET, 504, 
                                   retainAtom(message));
        httpServerAbort(connection, status != -ECLIENTRESET, 504, message);
        return 1;
    }

    do_log(D_SERVER_CONN, "C    %s:%d.\n",
           scrub(connection->server->name), connection->server->port);

    connection->connecting = 0;
    /* serverTrigger will take care of inserting any timeouts */
    httpServerTrigger(connection->server);
    return 1;
}