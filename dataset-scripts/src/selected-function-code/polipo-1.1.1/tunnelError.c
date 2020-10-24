static int
tunnelError(TunnelPtr tunnel, int code, AtomPtr message)
{
    int n;
    if(tunnel->fd2 > 0) {
        CLOSE(tunnel->fd2);
        tunnel->fd2 = -1;
    }

    if(tunnel->buf2.buf == NULL)
        tunnel->buf2.buf = get_chunk();
    if(tunnel->buf2.buf == NULL)
        goto fail;

    n = httpWriteErrorHeaders(tunnel->buf2.buf, CHUNK_SIZE - 1, 0,
                              1, code, message, 1, NULL,
                              NULL, 0, NULL);

    if(n <= 0) goto fail;

    tunnel->buf2.head = n;

    tunnelDispatch(tunnel);
    return 1;

 fail:
    CLOSE(tunnel->fd1);
    tunnel->fd1 = -1;
    tunnelDispatch(tunnel);
    return 1;
}