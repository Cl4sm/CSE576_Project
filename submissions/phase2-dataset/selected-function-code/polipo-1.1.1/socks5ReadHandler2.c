static int
socks5ReadHandler2(int status,
                   FdEventHandlerPtr event,
                   StreamRequestPtr srequest)
{
    SocksRequestPtr request = srequest->data;

    if(status < 0)
        goto error;

    if(srequest->offset < 4) {
        if(status) {
            status = -ESOCKS_PROTOCOL;
            goto error;
        }
        return 0;
    }

    if(request->buf[0] != 5) {
        status = -ESOCKS_PROTOCOL;
        goto error;
    }

    if(request->buf[1] != 0) {
        status = -(ESOCKS5_BASE + request->buf[1]);
        goto error;
    }

    if(request->buf[3] != 1) {
        status = -ESOCKS_PROTOCOL;
        goto error;
    }

    if(srequest->offset < 10)
        return 0;

    request->handler(1, request);
    destroySocksRequest(request);
    return 1;

 error:
    CLOSE(request->fd);
    request->fd = -1;
    request->handler(status, request);
    destroySocksRequest(request);
    return 1;
}