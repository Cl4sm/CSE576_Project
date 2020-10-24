static int
socksConnectHandler(int status,
                    FdEventHandlerPtr event,
                    ConnectRequestPtr crequest)
{
    SocksRequestPtr request = crequest->data;
    int rc;

    if(status < 0) {
        request->handler(status, request);
        destroySocksRequest(request);
        return 1;
    }

    assert(request->fd < 0);
    request->fd = crequest->fd;
    socksProxyAddressIndex = crequest->index;

    rc = setNodelay(request->fd, 1);
    if(rc < 0)
        do_log_error(L_WARN, errno, "Couldn't disable Nagle's algorithm");

    if(socksProxyType == aSocks4a) {
        request->buf = malloc(8 +
                              socksUserName->length + 1 +
                              request->name->length + 1);
        if(request->buf == NULL) {
            CLOSE(request->fd);
            request->fd = -1;
            request->handler(-ENOMEM, request);
            destroySocksRequest(request);
            return 1;
        }

        request->buf[0] = 4;        /* VN */
        request->buf[1] = 1;        /* CD = REQUEST */
        request->buf[2] = (request->port >> 8) & 0xFF;
        request->buf[3] = request->port & 0xFF;
        request->buf[4] = request->buf[5] = request->buf[6] = 0;
        request->buf[7] = 3;

        memcpy(request->buf + 8, socksUserName->string, socksUserName->length);
        request->buf[8 + socksUserName->length] = '\0';

        memcpy(request->buf + 8 + socksUserName->length + 1,
               request->name->string, request->name->length);
        request->buf[8 + socksUserName->length + 1 + request->name->length] =
            '\0';

        do_stream(IO_WRITE, request->fd, 0, request->buf,
                  8 + socksUserName->length + 1 + request->name->length + 1,
                  socksWriteHandler, request);
    } else if(socksProxyType == aSocks5) {
        request->buf = malloc(8); /* 8 needed for the subsequent read */
        if(request->buf == NULL) {
            CLOSE(request->fd);
            request->fd = -1;
            request->handler(-ENOMEM, request);
            destroySocksRequest(request);
            return 1;
        }

        request->buf[0] = 5;             /* ver */
        request->buf[1] = 1;             /* nmethods */
        request->buf[2] = 0;             /* no authentication required */
        do_stream(IO_WRITE, request->fd, 0, request->buf, 3,
                  socksWriteHandler, request);
    } else {
        request->handler(-EUNKNOWN, request);
    }
    return 1;
}