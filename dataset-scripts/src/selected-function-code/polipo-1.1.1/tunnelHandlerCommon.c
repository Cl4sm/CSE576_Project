static int
tunnelHandlerCommon(int fd, TunnelPtr tunnel)
{
    const char *message = "HTTP/1.1 200 Tunnel established\r\n\r\n";

    tunnel->fd2 = fd;

    if(parentHost)
        return tunnelHandlerParent(fd, tunnel);

    if(tunnel->buf2.buf == NULL)
        tunnel->buf2.buf = get_chunk();
    if(tunnel->buf2.buf == NULL) {
        CLOSE(fd);
        tunnelError(tunnel, 501, internAtom("Couldn't allocate buffer"));
        return 1;
    }

    memcpy(tunnel->buf2.buf, message, MIN(CHUNK_SIZE - 1, strlen(message)));
    tunnel->buf2.head = MIN(CHUNK_SIZE - 1, strlen(message));

    tunnelDispatch(tunnel);
    return 1;
}