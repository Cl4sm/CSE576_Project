FdEventHandlerPtr
do_connect(AtomPtr addr, int index, int port,
           int (*handler)(int, FdEventHandlerPtr, ConnectRequestPtr),
           void *data)
{
    ConnectRequestRec request;
    FdEventHandlerPtr event;
    int done, fd, af;

    assert(addr->length > 0 && addr->string[0] == DNS_A);
    assert(addr->length % sizeof(HostAddressRec) == 1);

    if(index >= (addr->length - 1)/ sizeof(HostAddressRec))
        index = 0;

    request.firstindex = index;
    request.port = port;
    request.handler = handler;
    request.data = data;
 again:
    af = addr->string[1 + index * sizeof(HostAddressRec)];
    fd = serverSocket(af);

    request.fd = fd;
    request.af = af;
    request.addr = addr;
    request.index = index;

    if(fd < 0) {
        int n = (addr->length - 1) / sizeof(HostAddressRec);
        if(errno == EAFNOSUPPORT || errno == EPROTONOSUPPORT) {
            if((index + 1) % n != request.firstindex) {
                index = (index + 1) % n;
                goto again;
            }
        }
        do_log_error(L_ERROR, errno, "Couldn't create socket");
        done = (*handler)(-errno, NULL, &request);
        assert(done);
        return NULL;
    }

    /* POLLIN is apparently needed on Windows */
    event = registerFdEvent(fd, POLLIN | POLLOUT,
                            do_scheduled_connect,
                            sizeof(ConnectRequestRec), &request);
    if(event == NULL) {
        done = (*handler)(-ENOMEM, NULL, &request);
        assert(done);
        return NULL;
    }

    done = event->handler(0, event);
    if(done) {
        unregisterFdEvent(event);
        return NULL;
    }
    return event;
}