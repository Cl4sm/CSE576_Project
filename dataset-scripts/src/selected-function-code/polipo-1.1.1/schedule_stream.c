FdEventHandlerPtr
schedule_stream(int operation, int fd, int offset,
                char *header, int hlen,
                char *buf, int len, char *buf2, int len2, char *buf3, int len3,
                char **buf_location,
                int (*handler)(int, FdEventHandlerPtr, StreamRequestPtr),
                void *data)
{
    StreamRequestRec request;
    FdEventHandlerPtr event;
    int done;

    request.operation = operation;
    request.fd = fd;
    if(len3) {
        assert(hlen == 0 && buf_location == NULL);
        request.u.b.len3 = len3;
        request.u.b.buf3 = buf3;
        request.operation |= IO_BUF3;
    } else if(buf_location) {
        assert(hlen == 0);
        request.u.l.buf_location = buf_location;
        request.operation |= IO_BUF_LOCATION;
    } else {
        request.u.h.hlen = hlen;
        request.u.h.header = header;
    }
    request.buf = buf;
    request.len = len;
    request.buf2 = buf2;
    request.len2 = len2;
    if((operation & IO_CHUNKED) || 
       (!(request.operation & (IO_BUF3 | IO_BUF_LOCATION)) && hlen > 0)) {
        assert(offset == 0);
        request.offset = -hlen;
        if(operation & IO_CHUNKED)
            request.offset += -chunkHeaderLen(len + len2);
    } else {
        request.offset = offset;
    }
    request.handler = handler;
    request.data = data;
    event = makeFdEvent(fd, 
                        (operation & IO_MASK) == IO_WRITE ?
                        POLLOUT : POLLIN, 
                        do_scheduled_stream, 
                        sizeof(StreamRequestRec), &request);
    if(!event) {
        done = (*handler)(-ENOMEM, NULL, &request);
        assert(done);
        return NULL;
    }

    if(!(operation & IO_NOTNOW)) {
        done = event->handler(0, event);
        if(done) {
            free(event);
            return NULL;
        }
    } 

    if(operation & IO_IMMEDIATE) {
        assert(hlen == 0 && !(operation & IO_CHUNKED));
        done = (*handler)(0, event, &request);
        if(done) {
            free(event);
            return NULL;
        }
    }
    event = registerFdEventHelper(event);
    return event;
}