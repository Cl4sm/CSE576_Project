int
httpServerSideRequest(HTTPServerPtr server)
{
    HTTPRequestPtr request = server->request;
    HTTPConnectionPtr connection;
    HTTPRequestPtr requestor = request->request;
    HTTPConnectionPtr client = requestor->connection;
    int rc, i, freeslots, idle, connecting;

    assert(REQUEST_SIDE(request));

    connection = NULL;
    freeslots = 0;
    idle = -1;
    connecting = 0;

    /* Find a fresh connection */
    for(i = 0; i < server->numslots; i++) {
        if(!server->connection[i])
            freeslots++;
        else if(!server->connection[i]->connecting) {
            if(!server->connection[i]->request) {
                if(server->connection[i]->serviced == 0) {
                    if(server->idleHandler[i])
                        unregisterFdEvent(server->idleHandler[i]);
                    server->idleHandler[i] = NULL;
                    connection = server->connection[i];
                    break;
                } else {
                    idle = i;
                }
            }
        } else {
            connecting++;
        }
    }

    if(!connection) {
        /* Make sure that a fresh connection will be established at some
           point, then wait until httpServerTrigger calls us again. */
        if(freeslots) {
            httpServerConnection(server);
        } else {
            if(idle >= 0) {
                /* Shutdown a random idle connection */
                pokeFdEvent(server->connection[idle]->fd, 
                            -EDOSHUTDOWN, POLLIN | POLLOUT);
            }
        }
        return 0;
    }

    rc = httpWriteRequest(connection, request, client->bodylen);
    if(rc < 0) {
        do_log(L_ERROR, "Couldn't write POST or PUT request.\n");
        httpServerAbortRequest(request, rc != -ECLIENTRESET, 502,
                               internAtom("Couldn't write request"));
        return 0;
    }
    server->request = request->next;
    request->next = NULL;
    if(server->request == NULL)
        server->request_last = NULL;
    httpQueueRequest(connection, request);
    connection->pipelined = 1;
    request->time0 = current_time;
    connection->reqoffset = 0;
    connection->bodylen = client->bodylen;
    httpServerDoSide(connection);
    return 1;
}