static void
discardServer(HTTPServerPtr server)
{
    HTTPServerPtr previous;
    assert(!server->request);

    if(server == servers)
        servers = server->next;
    else {
        previous = servers;
        while(previous->next != server)
            previous = previous->next;
        previous->next = server->next;
    }

    if(server->connection)
        free(server->connection);
    if(server->idleHandler)
        free(server->idleHandler);
    if(server->name)
        free(server->name);

    free(server);
}