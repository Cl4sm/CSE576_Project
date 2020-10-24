static void
bufRead(int fd, CircularBufferPtr buf,
        int (*handler)(int, FdEventHandlerPtr, StreamRequestPtr),
        void *data)
{
    int tail;

    if(buf->tail == 0)
        tail = CHUNK_SIZE - 1;
    else
        tail = buf->tail - 1;

    if(buf->head == 0)
        do_stream_buf(IO_READ | IO_NOTNOW,
                      fd, 0,
                      &buf->buf, tail,
                      handler, data);
    else if(buf->tail > buf->head)
        do_stream(IO_READ | IO_NOTNOW,
                  fd, buf->head,
                  buf->buf, tail,
                  handler, data);
    else 
        do_stream_2(IO_READ | IO_NOTNOW,
                    fd, buf->head,
                    buf->buf, CHUNK_SIZE,
                    buf->buf, tail,
                    handler, data);
}