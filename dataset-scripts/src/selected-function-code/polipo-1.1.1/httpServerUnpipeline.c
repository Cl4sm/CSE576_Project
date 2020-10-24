void
httpServerUnpipeline(HTTPRequestPtr request)
{
    HTTPConnectionPtr connection = request->connection;
    HTTPServerPtr server = connection->server;

    request->flags &= ~REQUEST_PERSISTENT;
    if(request->next) {
        HTTPRequestPtr req;
        do_log(L_WARN,
               "Restarting pipeline to %s:%d.\n", 
               scrub(connection->server->name), connection->server->port);
        req = request->next;
        while(req) {
            req->connection = NULL;
            req = req->next;
        }
        if(server->request)
            connection->request_last->next = server->request;
        else
            server->request_last = connection->request_last;
        server->request = request->next;
        request->next = NULL;
        connection->request_last = request;
    }
    connection->pipelined = httpConnectionPipelined(connection);
}