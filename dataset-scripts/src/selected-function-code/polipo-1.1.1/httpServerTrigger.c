int
httpServerTrigger(HTTPServerPtr server)
{
    HTTPConnectionPtr connection;
    HTTPRequestPtr request;
    int idle, n, i, rc, numidle;

    while(server->request) {
        httpServerDiscardRequests(server);

        if(!server->request)
            break;

        if(REQUEST_SIDE(server->request)) {
            rc = httpServerSideRequest(server);
            /* If rc is 0, httpServerSideRequest didn't dequeue this
               request.  Go through the scheduling loop again, come
               back later. */
            if(rc <= 0) break;
            continue;
        }
        connection = httpServerGetConnection(server, &numidle);
        if(!connection) break;

        /* If server->pipeline <= 0, we don't do pipelining.  If
           server->pipeline is 1, then we are ready to start probing
           for pipelining on the server; we then send exactly two
           requests in what is hopefully a single packet to check
           whether the server has the nasty habit of discarding its
           input buffers after each request.
           If server->pipeline is 2 or 3, the pipelining probe is in
           progress on this server, and we don't pipeline anything
           until it succeeds.  When server->pipeline >= 4, pipelining
           is believed to work on this server. */
        if(server->version != HTTP_11 || server->pipeline <= 0 ||
           server->pipeline == 2 || server->pipeline == 3) {
            if(connection->pipelined == 0)
                n = 1;
            else
                n = 0;
        } else if(server->pipeline == 1) {
            if(connection->pipelined == 0)
                n = MIN(2, maxPipelineTrain);
            else
                n = 0;
        } else {
            n = maxPipelineTrain;
        }

        /* Don't pipeline if there are more idle connections */
        if(numidle >= 2)
            n = MIN(n, 1);
    
        idle = !connection->pipelined;
        i = 0;
        while(server->request && connection->pipelined < n) {
            httpServerDiscardRequests(server);
            if(!server->request) break;
            request = server->request;
            assert(request->request->request == request);
            rc = httpWriteRequest(connection, request, -1);
            if(rc < 0) {
                if(i == 0)
                    httpServerAbortRequest(request, rc != -ECLIENTRESET, 502,
                                           internAtom("Couldn't "
                                                      "write request"));
                break;
            }
            do_log(D_SERVER_CONN, "W: ");
            do_log_n(D_SERVER_CONN, 
                     request->object->key, request->object->key_size);
            do_log(D_SERVER_CONN, " (%d)\n", request->method);
            if(connection->pipelined > 0)
                request->flags |= REQUEST_PIPELINED;
            request->time0 = current_time;
            i++;
            server->request = request->next;
            request->next = NULL;
            if(server->request == NULL)
                server->request_last = NULL;
            httpQueueRequest(connection, request);
            connection->pipelined++;
        }
        if(server->persistent > 0 && server->pipeline == 1 && i >= 2)
            server->pipeline = 2;

        if(i > 0) httpServerSendRequest(connection);

        if(idle && connection->pipelined > 0)
            httpServerReply(connection, 0);

        if(i == 0) break;
    }

    for(i = 0; i < server->maxslots; i++) {
        if(server->connection[i] &&
           !server->connection[i]->connecting &&
           !server->connection[i]->request) {
            /* Artificially age any fresh connections that aren't used
               straight away; this is necessary for the logic for POST and 
               the logic that determines whether a given request should be 
               restarted. */
            if(server->connection[i]->serviced == 0)
                server->connection[i]->serviced = 1;
            if(!server->idleHandler[i])
                server->idleHandler[i] = 
                    registerFdEvent(server->connection[i]->fd, POLLIN,
                                    httpServerIdleHandler,
                                    sizeof(HTTPConnectionPtr),
                                    &server->connection[i]);
            if(!server->idleHandler[i]) {
                do_log(L_ERROR, "Couldn't register idle handler.\n");
                httpServerFinish(server->connection[i], 1, 0);
            }
            httpSetTimeout(server->connection[i], serverIdleTimeout);
        }
    }

    return 1;
}