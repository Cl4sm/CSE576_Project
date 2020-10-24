int
httpServerHandler(int status,
                  FdEventHandlerPtr event,
                  StreamRequestPtr srequest)
{
    HTTPConnectionPtr connection = srequest->data;

    assert(connection->request->object->flags & OBJECT_INPROGRESS);

    if(connection->reqlen == 0) {
        do_log(D_SERVER_REQ, "Writing aborted on 0x%lx\n",
               (unsigned long)connection);
        goto fail;
    }

    if(status == 0 && !streamRequestDone(srequest)) {
        httpSetTimeout(connection, serverTimeout);
        return 0;
    }

    httpConnectionDestroyReqbuf(connection);

    if(status) {
        if(connection->serviced >= 1) {
            httpServerRestart(connection);
            return 1;
        }
        if(status < 0 && status != -ECONNRESET && status != -EPIPE)
            do_log_error(L_ERROR, -status,
                         "Couldn't send request to server");
        goto fail;
    }

    return 1;

 fail:
    httpConnectionDestroyReqbuf(connection);
    shutdown(connection->fd, 2);
    pokeFdEvent(connection->fd, -EDOSHUTDOWN, POLLIN);
    httpSetTimeout(connection, 60);
    return 1;
}