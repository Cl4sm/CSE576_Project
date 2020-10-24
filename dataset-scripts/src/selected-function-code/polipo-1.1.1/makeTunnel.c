static TunnelPtr
makeTunnel(int fd, char *buf, int offset, int len)
{
    TunnelPtr tunnel;
    assert(offset < CHUNK_SIZE);

    tunnel = malloc(sizeof(TunnelRec));
    if(tunnel == NULL)
        return NULL;

    tunnel->hostname = NULL;
    tunnel->port = -1;
    tunnel->flags = 0;
    tunnel->fd1 = fd;
    tunnel->fd2 = -1;
    tunnel->buf1.buf = buf;
    if(offset == len) {
        tunnel->buf1.tail = 0;
        tunnel->buf1.head = 0;
    } else {
        tunnel->buf1.tail = offset;
        tunnel->buf1.head = len;
    }
    tunnel->buf2.buf = NULL;
    tunnel->buf2.tail = 0;
    tunnel->buf2.head = 0;
    return tunnel;
}