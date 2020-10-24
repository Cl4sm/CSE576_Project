static int
tunnelHandlerParent(int fd, TunnelPtr tunnel)
{
    char *message;
    int n;

    if(tunnel->buf1.buf == NULL)
        tunnel->buf1.buf = get_chunk();
    if(tunnel->buf1.buf == NULL) {
        message = "Couldn't allocate buffer";
        goto fail;
    }
    if(tunnel->buf1.tail != tunnel->buf1.head) {
        message = "Pipelined connect to parent proxy not implemented";
        goto fail;
    }

    n = snnprintf(tunnel->buf1.buf, tunnel->buf1.tail, CHUNK_SIZE,
                  "CONNECT %s:%d HTTP/1.1",
                  tunnel->hostname->string, tunnel->port);
    if (parentAuthCredentials)
        n = buildServerAuthHeaders(tunnel->buf1.buf, n, CHUNK_SIZE,
                                   parentAuthCredentials);
    n = snnprintf(tunnel->buf1.buf, n, CHUNK_SIZE, "\r\n\r\n");

    if(n < 0) {
        message = "Buffer overflow";
        goto fail;
    }
    tunnel->buf1.head = n;
    tunnelDispatch(tunnel);
    return 1;

 fail:
    CLOSE(fd);
    tunnel->fd2 = -1;
    tunnelError(tunnel, 501, internAtom(message));
    return 1;
}