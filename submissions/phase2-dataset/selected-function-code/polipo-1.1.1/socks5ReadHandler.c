static int
socks5ReadHandler(int status,
                  FdEventHandlerPtr event,
                  StreamRequestPtr srequest)
{
    SocksRequestPtr request = srequest->data;

    if(status < 0)
        goto error;

    if(srequest->offset < 2) {
        if(status) {
            status = -ESOCKS_PROTOCOL;
            goto error;
        }
        return 0;
    }

    if(request->buf[0] != 5 || request->buf[1] != 0) {
        status = -ESOCKS_PROTOCOL;
        goto error;
    }

    free(request->buf);
    request->buf = malloc(5 + request->name->length + 2);
    if(request->buf == NULL) {
        status = -ENOMEM;
        goto error;
    }

    request->buf[0] = 5;        /* ver */
    request->buf[1] = 1;        /* cmd */
    request->buf[2] = 0;        /* rsv */
    request->buf[3] = 3;        /* atyp */
    request->buf[4] = request->name->length;
    memcpy(request->buf + 5, request->name->string, request->name->length);
    request->buf[5 + request->name->length] = (request->port >> 8) & 0xFF;
    request->buf[5 + request->name->length + 1] = request->port & 0xFF;

    do_stream(IO_WRITE, request->fd, 0,
              request->buf, 5 + request->name->length + 2,
              socks5WriteHandler, request);
    return 1;

 error:
    CLOSE(request->fd);
    request->fd = -1;
    request->handler(status, request);
    destroySocksRequest(request);
    return 1;
}