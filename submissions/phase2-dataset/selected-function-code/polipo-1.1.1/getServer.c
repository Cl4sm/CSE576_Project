static HTTPServerPtr
getServer(char *name, int port, int proxy)
{
    HTTPServerPtr server;
    int i;

    server = servers;
    while(server) {
        if(strcmp(server->name, name) == 0 && server->port == port &&
           server->isProxy == proxy) {
            if(httpServerIdle(server) &&
               server->time +  serverExpireTime < current_time.tv_sec) {
                discardServer(server);
                server = NULL;
                break;
            } else {
                server->time = current_time.tv_sec;
                return server;
            }
        }
        server = server->next;
    }
    
    server = malloc(sizeof(HTTPServerRec));
    if(server == NULL) {
        do_log(L_ERROR, "Couldn't allocate server.\n");
        return NULL;
    }

    server->connection = malloc(serverMaxSlots * sizeof(HTTPConnectionPtr));
    if(server->connection == NULL) {
        do_log(L_ERROR, "Couldn't allocate server.\n");
        free(server);
        return NULL;
    }

    server->idleHandler = malloc(serverMaxSlots * sizeof(FdEventHandlerPtr));
    if(server->connection == NULL) {
        do_log(L_ERROR, "Couldn't allocate server.\n");
        free(server->connection);
        free(server);
        return NULL;
    }

    server->maxslots = serverMaxSlots;

    server->name = strdup(name);
    if(server->name == NULL) {
        do_log(L_ERROR, "Couldn't allocate server name.\n");
        free(server);
        return NULL;
    }

    server->port = port;
    server->addrindex = 0;
    server->isProxy = proxy;
    server->version = HTTP_UNKNOWN;
    server->persistent = 0;
    server->pipeline = 0;
    server->time = current_time.tv_sec;
    server->rtt = -1;
    server->rate = -1;
    server->numslots = MIN(serverSlots, server->maxslots);
    for(i = 0; i < server->maxslots; i++) {
        server->connection[i] = NULL;
        server->idleHandler[i] = NULL;
    }
    server->request = NULL;
    server->request_last = NULL;
    server->lies = 0;

    server->next = servers;
    servers = server;
    return server;
}