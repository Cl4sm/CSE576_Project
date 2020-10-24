int
httpServerIdleHandler(int a, FdEventHandlerPtr event)
{
    HTTPConnectionPtr connection = *(HTTPConnectionPtr*)event->data;
    HTTPServerPtr server = connection->server;
    int i;

    assert(!connection->request);

    do_log(D_SERVER_CONN, "Idle connection to %s:%d died.\n", 
           scrub(connection->server->name), connection->server->port);

    for(i = 0; i < server->maxslots; i++) {
        if(connection == server->connection[i]) {
            server->idleHandler[i] = NULL;
            break;
        }
    }
    assert(i < server->maxslots);

    httpServerAbort(connection, 1, 504, internAtom("Timeout"));
    return 1;
}