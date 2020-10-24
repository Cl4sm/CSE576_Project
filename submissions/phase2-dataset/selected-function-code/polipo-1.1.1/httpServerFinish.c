void
httpServerFinish(HTTPConnectionPtr connection, int s, int offset)
{
    HTTPServerPtr server = connection->server;
    HTTPRequestPtr request = connection->request;
    int i;

    if(request) {
        assert(connection->pipelined >= 1);
        assert((connection->pipelined > 1) == (request->next != NULL));
    } else {
        assert(connection->pipelined == 0);
    }

    if(!s && (!connection->request ||
                  !(connection->request->flags & REQUEST_PERSISTENT)))
        s = 1;

    if(connection->serviced >= maxConnectionRequests ||
       connection->time < current_time.tv_sec - maxConnectionAge)
        s = 1;

    if(connection->reqbuf) {
        /* As most normal requests go out in a single packet, this is
           extremely unlikely to happen.  As for POST/PUT requests,
           they are not pipelined, so this can only happen if the
           server sent an error reply early. */
        assert(connection->fd >= 0);
        shutdown(connection->fd, 1);
        pokeFdEvent(connection->fd, -EDOSHUTDOWN, POLLOUT);
        httpServerDelayedFinish(connection);
        goto done;
    }

    if(request) {
        /* Update statistics about the server */
        int size = -1, d = -1, rtt = -1, rate = -1;
        if(connection->offset > 0 && request->from >= 0)
            size = connection->offset - request->from;
        if(request->time1.tv_sec != null_time.tv_sec) {
            d = timeval_minus_usec(&current_time, &request->time1);
            if(!(request->flags & REQUEST_PIPELINED) &&
               request->time0.tv_sec != null_time.tv_sec)
                rtt = timeval_minus_usec(&request->time1, &request->time0);
            if(size >= 8192 && d > 50000)
                rate = ((double)size / (double)d) * 1000000.0 + 0.5;
        }
        request->time0 = null_time;
        request->time1 = null_time;

        if(rtt >= 0) {
            if(server->rtt >= 0)
                server->rtt = (3 * server->rtt + rtt + 2) / 4;
            else
                server->rtt = rtt;
        }
        if(rate >= 0) {
            if(server->rate >= 0)
                server->rate = (3 * server->rate + rate + 2) / 4;
            else
                server->rate = rate;
        }

        httpDequeueRequest(connection);
        connection->pipelined--;
        request->object->flags &= ~(OBJECT_INPROGRESS | OBJECT_VALIDATING);
        if(request->request) {
            request->request->request = NULL;
            request->request = NULL;
        }
        releaseNotifyObject(request->object);
        request->object = NULL;
        httpDestroyRequest(request);
    }

    do_log(D_SERVER_CONN, "Done with server %s:%d connection (%d)\n",
           scrub(connection->server->name), connection->server->port, s);

    assert(offset <= connection->len);

    if(!s) {
        if(offset < connection->len) {
            assert(connection->buf != NULL);
            if(!connection->pipelined) {
                do_log(L_WARN, 
                       "Closing connection to %s:%d: "
                       "%d stray bytes of data.\n",
                       scrub(server->name), server->port,
                       connection->len - offset);
                s = 1;
            } else {
                memmove(connection->buf, connection->buf + offset,
                        connection->len - offset);
                connection->len = connection->len - offset;
                if((connection->flags & CONN_BIGBUF) &&
                   connection->len <= CHUNK_SIZE)
                    httpConnectionUnbigify(connection);
            }
        } else {
            connection->len = 0;
        }
    }

    connection->server->time = current_time.tv_sec;
    connection->serviced++;

    if(s) {
        if(connection->timeout)
            cancelTimeEvent(connection->timeout);
        connection->timeout = NULL;
        httpConnectionDestroyBuf(connection);
        if(connection->fd >= 0)
            CLOSE(connection->fd);
        connection->fd = -1;
        server->persistent -= 1;
        if(server->persistent < -5)
            server->numslots = MIN(server->maxslots, serverMaxSlots);
        if(connection->request) {
            HTTPRequestPtr req;
            do_log(D_SERVER_CONN, "Restarting pipeline to %s:%d.\n",
                   scrub(server->name), server->port);
            if(server->pipeline == 2)
                server->pipeline -= 20;
            else
                server->pipeline -= 5;
            req = connection->request;
            while(req) {
                req->connection = NULL;
                req = req->next;
            }
            if(server->request)
                connection->request_last->next = server->request;
            else
                server->request_last = connection->request_last;
            server->request = connection->request;
            connection->request = NULL;
            connection->request_last = NULL;
        }
        /* Make sure we don't get confused into thinking a probe
           is in progress. */
        if(server->pipeline == 2 || server->pipeline == 3)
            server->pipeline = 1;
        for(i = 0; i < server->maxslots; i++)
            if(connection == server->connection[i])
                break;
        assert(i < server->maxslots);
        if(server->idleHandler[i])
            unregisterFdEvent(server->idleHandler[i]);
        server->idleHandler[i] = NULL;
        server->connection[i] = NULL;
        free(connection);
    } else {
        server->persistent += 1;
        if(server->persistent > 0)
            server->numslots = MIN(server->maxslots,
                                   server->version == HTTP_10 ?
                                   serverSlots1 : serverSlots);
        httpSetTimeout(connection, serverTimeout);
        /* See httpServerTrigger */
        if(connection->pipelined ||
           (server->version == HTTP_11 && server->pipeline <= 0) ||
           (server->pipeline == 3)) {
            server->pipeline++;
        }
        if(connection->pipelined) {
            httpServerReply(connection, 1);
        } else {
            httpConnectionDestroyBuf(connection);
        }
    }

 done:
    httpServerTrigger(server);
}