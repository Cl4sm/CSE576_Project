FdEventHandlerPtr
create_listener(char *address, int port,
                int (*handler)(int, FdEventHandlerPtr, AcceptRequestPtr),
                void *data)
{
    int fd, rc;
    int one = 1;
    int done;
    struct sockaddr_in addr;
#ifdef HAVE_IPv6
    int inet6 = 1;
    struct sockaddr_in6 addr6;
#else
    int inet6 = 0;
#endif

    if(inet6 && address) {
        struct in_addr buf;
        rc = inet_aton(address, &buf);
        if(rc == 1)
            inet6 = 0;
    }
    fd = -1;
    errno = EAFNOSUPPORT;

#ifdef HAVE_IPv6
    if(inet6) {
        fd = socket(PF_INET6, SOCK_STREAM, 0);
    }
#endif

    if(fd < 0 && (errno == EPROTONOSUPPORT || errno == EAFNOSUPPORT)) {
        inet6 = 0;
        fd = socket(PF_INET, SOCK_STREAM, 0);
    }

    if(fd < 0) {
        done = (*handler)(-errno, NULL, NULL);
        assert(done);
        return NULL;
    }

#ifndef WIN32
    /* on WIN32 SO_REUSEADDR allows two sockets bind to the same port */
    rc = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(one));
    if(rc < 0) do_log_error(L_WARN, errno, "Couldn't set SO_REUSEADDR");
#endif

    if(inet6) {
#ifdef HAVE_IPv6
        rc = setV6only(fd, 0);
        if(rc < 0)
            /* Reportedly OpenBSD returns an error for that.  So only
               log it as a debugging message. */
            do_log_error(D_CLIENT_CONN, errno, "Couldn't reset IPV6_V6ONLY");

        memset(&addr6, 0, sizeof(addr6));
        rc = inet_pton(AF_INET6, address, &addr6.sin6_addr);
        if(rc != 1) {
            done = (*handler)(rc == 0 ? -ESYNTAX : -errno, NULL, NULL);
            assert(done);
            return NULL;
        }
        addr6.sin6_family = AF_INET6;
        addr6.sin6_port = htons(port);
        rc = bind(fd, (struct sockaddr*)&addr6, sizeof(addr6));
#else
        rc = -1;
        errno = EAFNOSUPPORT;
#endif
    } else {
        memset(&addr, 0, sizeof(addr));
        rc = inet_aton(address, &addr.sin_addr);
        if(rc != 1) {
            done = (*handler)(rc == 0 ? -ESYNTAX : -errno, NULL, NULL);
            assert(done);
            return NULL;
        }
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        rc = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    }

    if(rc < 0) {
        do_log_error(L_ERROR, errno, "Couldn't bind");
        CLOSE(fd);
        done = (*handler)(-errno, NULL, NULL);
        assert(done);
        return NULL;
    }

    rc = setNonblocking(fd, 1);
    if(rc < 0) {
        do_log_error(L_ERROR, errno, "Couldn't set non blocking mode");
        CLOSE(fd);
        done = (*handler)(-errno, NULL, NULL);
        assert(done);
        return NULL;
    }
        
    rc = listen(fd, 1024);
    if(rc < 0) {
        do_log_error(L_ERROR, errno, "Couldn't listen");
        CLOSE(fd);
        done = (*handler)(-errno, NULL, NULL);
        assert(done);
        return NULL;
    }

    do_log(L_INFO, "Established listening socket on port %d.\n", port);

    return schedule_accept(fd, handler, data);
}