NOEXPORT void init_local(CLI *c) {
    SOCKADDR_UNION addr;
    socklen_t addr_len;
    char *accepted_address;

    /* check if local_rfd is a socket and get peer address */
    addr_len=sizeof(SOCKADDR_UNION);
    c->local_rfd.is_socket=!getpeername(c->local_rfd.fd, &addr.sa, &addr_len);
    if(c->local_rfd.is_socket) {
        memcpy(&c->peer_addr.sa, &addr.sa, addr_len);
        c->peer_addr_len=addr_len;
        if(set_socket_options(c->local_rfd.fd, 1))
            s_log(LOG_WARNING, "Failed to set local socket options");
    } else {
        if(get_last_socket_error()!=S_ENOTSOCK) {
            sockerror("getpeerbyname (local_rfd)");
            longjmp(c->err, 1);
        }
    }

    /* check if local_wfd is a socket and get peer address */
    if(c->local_rfd.fd==c->local_wfd.fd) {
        c->local_wfd.is_socket=c->local_rfd.is_socket;
    } else {
        addr_len=sizeof(SOCKADDR_UNION);
        c->local_wfd.is_socket=!getpeername(c->local_wfd.fd, &addr.sa, &addr_len);
        if(c->local_wfd.is_socket) {
            if(!c->local_rfd.is_socket) { /* already retrieved */
                memcpy(&c->peer_addr.sa, &addr.sa, addr_len);
                c->peer_addr_len=addr_len;
            }
            if(set_socket_options(c->local_wfd.fd, 1))
                s_log(LOG_WARNING, "Failed to set local socket options");
        } else {
            if(get_last_socket_error()!=S_ENOTSOCK) {
                sockerror("getpeerbyname (local_wfd)");
                longjmp(c->err, 1);
            }
        }
    }

    /* neither of local descriptors is a socket */
    if(!c->local_rfd.is_socket && !c->local_wfd.is_socket) {
#ifndef USE_WIN32
        if(c->opt->option.transparent_src) {
            s_log(LOG_ERR, "Transparent source needs a socket");
            longjmp(c->err, 1);
        }
#endif
        s_log(LOG_NOTICE, "Service [%s] accepted connection", c->opt->servname);
        return;
    }

    /* authenticate based on retrieved IP address of the client */
    accepted_address=s_ntop(&c->peer_addr, c->peer_addr_len);
#ifdef USE_LIBWRAP
    libwrap_auth(c, accepted_address);
#endif /* USE_LIBWRAP */
    auth_user(c, accepted_address);
    s_log(LOG_NOTICE, "Service [%s] accepted connection from %s",
        c->opt->servname, accepted_address);
    str_free(accepted_address);
}
