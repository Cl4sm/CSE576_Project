int
do_scheduled_connect(int status, FdEventHandlerPtr event)
{
    ConnectRequestPtr request = (ConnectRequestPtr)&event->data;
    AtomPtr addr = request->addr;
    int done;
    int rc;
    HostAddressPtr host;
    struct sockaddr_in servaddr;
#ifdef HAVE_IPv6
    struct sockaddr_in6 servaddr6;
#endif

    assert(addr->length > 0 && addr->string[0] == DNS_A);
    assert(addr->length % sizeof(HostAddressRec) == 1);
    assert(request->index < (addr->length - 1) / sizeof(HostAddressRec));

    if(status) {
        done = request->handler(status, event, request);
        if(done) {
            releaseAtom(addr);
            request->addr = NULL;
            return 1;
        }
        return 0;
    }

 again:
    host = (HostAddressPtr)&addr->string[1 + 
                                         request->index * 
                                         sizeof(HostAddressRec)];
    if(host->af != request->af) {
        int newfd;
        /* Ouch.  Our socket has a different protocol than the host
           address. */
        CLOSE(request->fd);
        newfd = serverSocket(host->af);
        if(newfd < 0) {
            if(errno == EAFNOSUPPORT || errno == EPROTONOSUPPORT) {
                int n = request->addr->length / sizeof(HostAddressRec);
                if((request->index + 1) % n != request->firstindex) {
                    request->index = (request->index + 1) % n;
                    goto again;
                }
            }
            request->fd = -1;
            done = request->handler(-errno, event, request);
            assert(done);
            return 1;
        }
        if(newfd != request->fd) {
            request->fd = dup2(newfd, request->fd);
            CLOSE(newfd);
            if(request->fd < 0) {
                done = request->handler(-errno, event, request);
                assert(done);
                return 1;
            }
        }
        request->af = host->af;
    }
    switch(host->af) {
    case 4:
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(request->port);
        memcpy(&servaddr.sin_addr, &host->data, sizeof(struct in_addr));
        rc = connect(request->fd,
                     (struct sockaddr*)&servaddr, sizeof(servaddr));
        break;
    case 6:
#ifdef HAVE_IPv6
        memset(&servaddr6, 0, sizeof(servaddr6));
        servaddr6.sin6_family = AF_INET6;
        servaddr6.sin6_port = htons(request->port);
        memcpy(&servaddr6.sin6_addr, &host->data, sizeof(struct in6_addr));
        rc = connect(request->fd,
                     (struct sockaddr*)&servaddr6, sizeof(servaddr6));
#else
        rc = -1;
        errno = EAFNOSUPPORT;
#endif
        break;
    default:
        abort();
    }
        
    if(rc >= 0 || errno == EISCONN) {
        done = request->handler(1, event, request);
        assert(done);
        releaseAtom(request->addr);
        request->addr = NULL;
        return 1;
    }

    if(errno == EINPROGRESS || errno == EINTR) {
        return 0;
    } else if(errno == EFAULT || errno == EBADF) {
        abort();
    } else {
        int n = request->addr->length / sizeof(HostAddressRec);
        if((request->index + 1) % n != request->firstindex) {
            request->index = (request->index + 1) % n;
            goto again;
        }
        done = request->handler(-errno, event, request);
        assert(done);
        releaseAtom(request->addr);
        request->addr = NULL;
        return 1;
    }
}