void
httpServerRestart(HTTPConnectionPtr connection)
{
    HTTPServerPtr server = connection->server;
    HTTPRequestPtr request = connection->request;

    if(request) {
        HTTPRequestPtr req;
        if(request->next)
            do_log(L_WARN,
                   "Restarting pipeline to %s:%d.\n", 
                   scrub(connection->server->name), connection->server->port);
        req = request;
        while(req) {
            req->connection = NULL;
            req = req->next;
        }
        if(server->request)
            connection->request_last->next = server->request;
        else
            server->request_last = connection->request_last;
        server->request = request;
        connection->request = NULL;
        connection->request_last = NULL;
    }
    connection->pipelined = 0;
    httpServerFinish(connection, 1, 0);
}