static int
socks5WriteHandler(int status,
                   FdEventHandlerPtr event,
                   StreamRequestPtr srequest)
{
    SocksRequestPtr request = srequest->data;
    
    if(status < 0)
        goto error;

    if(!streamRequestDone(srequest)) {
        if(status) {
            status = -ESOCKS_PROTOCOL;
            goto error;
        }
        return 0;
    }

    do_stream(IO_READ | IO_NOTNOW, request->fd, 0, request->buf, 10,
              socks5ReadHandler2, request);
    return 1;

 error:
    request->handler(status, request);
    destroySocksRequest(request);
    return 1;
}