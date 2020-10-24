static int
socksReadHandler(int status,
                 FdEventHandlerPtr event,
                 StreamRequestPtr srequest)
{
    SocksRequestPtr request = srequest->data;

    if(status < 0)
        goto error;

    if(srequest->offset < 8) {
        if(status) {
            status = -ESOCKS_PROTOCOL;
            goto error;
        }
        return 0;
    }

    if(request->buf[0] != 0 || request->buf[1] != 90) {
        if(request->buf[1] >= 91 && request->buf[1] <= 93)
            status = -(ESOCKS_PROTOCOL + request->buf[1] - 90);
        else
            status = -ESOCKS_PROTOCOL;
        goto error;
    }

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