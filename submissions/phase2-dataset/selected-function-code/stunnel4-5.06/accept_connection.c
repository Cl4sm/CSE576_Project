    SOCKADDR_UNION addr;
    char *from_address;
    int s;
    socklen_t addrlen;

    addrlen=sizeof addr;
    for(;;) {
        s=s_accept(opt->fd, &addr.sa, &addrlen, 1, "local socket");
        if(s>=0) /* success! */
            break;
        switch(get_last_socket_error()) {
            case S_EINTR: /* interrupted by a signal */
                break; /* retry now */
            case S_EMFILE:
#ifdef S_ENFILE
            case S_ENFILE:
#endif
#ifdef S_ENOBUFS
            case S_ENOBUFS:
#endif
#ifdef S_ENOMEM
            case S_ENOMEM:
#endif
                return 1; /* temporary lack of resources */
            default:
                return 0; /* any other error */
        }
    }
    from_address=s_ntop(&addr, addrlen);
    s_log(LOG_DEBUG, "Service [%s] accepted (FD=%d) from %s",
        opt->servname, s, from_address);
    str_free(from_address);
#ifdef USE_FORK
    RAND_add("", 1, 0.0); /* each child needs a unique entropy pool */
#else
    if(max_clients && num_clients>=max_clients) {
        s_log(LOG_WARNING, "Connection rejected: too many clients (>=%d)",
            max_clients);
        closesocket(s);
        return 0;
    }
#endif
    if(create_client(opt->fd, s,
            alloc_client_session(opt, s, s), client_thread)) {
        s_log(LOG_ERR, "Connection rejected: create_client failed");
        closesocket(s);
        return 0;
    }
    return 0;
}
