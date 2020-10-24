int
httpServerConnection(HTTPServerPtr server)
{
    HTTPConnectionPtr connection;
    int i;

    connection = httpMakeConnection();
    if(connection == NULL) {
        do_log(L_ERROR, "Couldn't allocate server connection.\n");
        return -1;
    }
    connection->server = server;

    for(i = 0; i < server->numslots; i++) {
        if(!server->connection[i]) {
            server->connection[i] = connection;
            break;
        }
    }
    assert(i < server->numslots);
    
    connection->request = NULL;
    connection->request_last = NULL;

    do_log(D_SERVER_CONN, "C... %s:%d.\n",
           scrub(connection->server->name), connection->server->port);
    httpSetTimeout(connection, serverTimeout);
    if(socksParentProxy) {
        connection->connecting = CONNECTING_SOCKS;
        do_socks_connect(server->name, connection->server->port,
                         httpServerSocksHandler, connection);
    } else {
        connection->connecting = CONNECTING_DNS;
        do_gethostbyname(server->name, 0,
                         httpServerConnectionDnsHandler,
                         connection);
    }
    return 1;
}