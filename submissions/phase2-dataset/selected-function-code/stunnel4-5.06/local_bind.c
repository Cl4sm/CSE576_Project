#ifndef USE_WIN32
    int on;

    on=1;
#endif
    if(!c->bind_addr)
        return;
#if defined(USE_WIN32)
    /* do nothing */
#elif defined(__linux__)
    /* non-local bind on Linux */
    if(c->opt->option.transparent_src) {
        if(setsockopt(c->fd, SOL_IP, IP_TRANSPARENT, &on, sizeof on)) {
            sockerror("setsockopt IP_TRANSPARENT");
            if(setsockopt(c->fd, SOL_IP, IP_FREEBIND, &on, sizeof on))
                sockerror("setsockopt IP_FREEBIND");
            else
                s_log(LOG_INFO, "IP_FREEBIND socket option set");
        } else
            s_log(LOG_INFO, "IP_TRANSPARENT socket option set");
        /* ignore the error to retain Linux 2.2 compatibility */
        /* the error will be handled by bind(), anyway */
    }
#elif defined(IP_BINDANY) && defined(IPV6_BINDANY)
    /* non-local bind on FreeBSD */
    if(c->opt->option.transparent_src) {
        if(c->bind_addr->sa.sa_family==AF_INET) { /* IPv4 */
            if(setsockopt(c->fd, IPPROTO_IP, IP_BINDANY, &on, sizeof on)) {
                sockerror("setsockopt IP_BINDANY");
                longjmp(c->err, 1);
            }
        } else { /* IPv6 */
            if(setsockopt(c->fd, IPPROTO_IPV6, IPV6_BINDANY, &on, sizeof on)) {
                sockerror("setsockopt IPV6_BINDANY");
                longjmp(c->err, 1);
            }
        }
    }
#else
    /* unsupported platform */
    if(c->opt->option.transparent_src) {
        s_log(LOG_ERR, "Transparent proxy in remote mode is not supported"
            " on this platform");
        longjmp(c->err, 1);
    }
#endif

    if(ntohs(c->bind_addr->in.sin_port)>=1024) { /* security check */
        /* this is currently only possible with transparent_src */
        if(!bind(c->fd, &c->bind_addr->sa, addr_len(c->bind_addr))) {
            s_log(LOG_INFO, "local_bind succeeded on the original port");
            return; /* success */
        }
        if(get_last_socket_error()!=S_EADDRINUSE) {
            sockerror("local_bind (original port)");
            longjmp(c->err, 1);
        }
    }

    c->bind_addr->in.sin_port=htons(0); /* retry with ephemeral port */
    if(!bind(c->fd, &c->bind_addr->sa, addr_len(c->bind_addr))) {
        s_log(LOG_INFO, "local_bind succeeded on an ephemeral port");
        return; /* success */
    }
    sockerror("local_bind (ephemeral port)");
    longjmp(c->err, 1);
}
