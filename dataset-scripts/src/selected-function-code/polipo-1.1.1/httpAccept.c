int
httpAccept(int fd, FdEventHandlerPtr event, AcceptRequestPtr request)
{
    int rc;
    HTTPConnectionPtr connection;
    TimeEventHandlerPtr timeout;

    if(fd < 0) {
        if(-fd == EINTR || -fd == EAGAIN || -fd == EWOULDBLOCK)
            return 0;
        do_log_error(L_ERROR, -fd, "Couldn't establish listening socket");
        if(-fd == EMFILE || -fd == ENOMEM || -fd == ENOBUFS) {
            TimeEventHandlerPtr again = NULL;
            do_log(L_WARN, "Refusing client connections for one second.\n");
            free_chunk_arenas();
            again = scheduleTimeEvent(1, httpAcceptAgain, 
                                      sizeof(request->fd), &request->fd);
            if(!again) {
                do_log(L_ERROR, "Couldn't schedule accept -- sleeping.\n");
                sleep(1);
                again = scheduleTimeEvent(1, httpAcceptAgain, 
                                          sizeof(request->fd), &request->fd);
                if(!again) {
                    do_log(L_ERROR, "Couldn't schedule accept -- aborting.\n");
                    polipoExit();
                }
            }
            return 1;
        } else {
            polipoExit();
            return 1;
        }
    }

    if(allowedNets) {
        if(netAddressMatch(fd, allowedNets) != 1) {
            do_log(L_WARN, "Refusing connection from unauthorised net\n");
            CLOSE(fd);
            return 0;
        }
    }

    rc = setNonblocking(fd, 1);
    if(rc < 0) {
        do_log_error(L_WARN, errno, "Couldn't set non blocking mode");
        CLOSE(fd);
        return 0;
    }
    rc = setNodelay(fd, 1);
    if(rc < 0) 
        do_log_error(L_WARN, errno, "Couldn't disable Nagle's algorithm");

    connection = httpMakeConnection();

    timeout = scheduleTimeEvent(clientTimeout, httpTimeoutHandler,
                                sizeof(connection), &connection);
    if(!timeout) {
        CLOSE(fd);
        free(connection);
        return 0;
    }

    connection->fd = fd;
    connection->timeout = timeout;

    do_log(D_CLIENT_CONN, "Accepted client connection 0x%lx\n",
           (unsigned long)connection);

    connection->flags = CONN_READER;

    do_stream_buf(IO_READ | IO_NOTNOW, connection->fd, 0,
                  &connection->reqbuf, CHUNK_SIZE,
                  httpClientHandler, connection);
    return 0;
}