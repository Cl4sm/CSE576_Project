int s_connect(CLI *c, SOCKADDR_UNION *addr, socklen_t addrlen) {
    int error;
    char *dst;

    dst=s_ntop(addr, addrlen);
    s_log(LOG_INFO, "s_connect: connecting %s", dst);

    if(!connect(c->fd, &addr->sa, addrlen)) {
        s_log(LOG_NOTICE, "s_connect: connected %s", dst);
        str_free(dst);
        return 0; /* no error -> success (on some OSes over the loopback) */
    }
    error=get_last_socket_error();
    if(error!=S_EINPROGRESS && error!=S_EWOULDBLOCK) {
        s_log(LOG_ERR, "s_connect: connect %s: %s (%d)",
            dst, s_strerror(error), error);
        str_free(dst);
        return -1;
    }

    s_log(LOG_DEBUG, "s_connect: s_poll_wait %s: waiting %d seconds",
        dst, c->opt->timeout_connect);
    s_poll_init(c->fds);
    s_poll_add(c->fds, c->fd, 1, 1);
    switch(s_poll_wait(c->fds, c->opt->timeout_connect, 0)) {
    case -1:
        error=get_last_socket_error();
        s_log(LOG_ERR, "s_connect: s_poll_wait %s: %s (%d)",
            dst, s_strerror(error), error);
        str_free(dst);
        return -1;
    case 0:
        s_log(LOG_ERR, "s_connect: s_poll_wait %s:"
            " TIMEOUTconnect exceeded", dst);
        str_free(dst);
        return -1;
    default:
        error=get_socket_error(c->fd);
        if(error) {
            s_log(LOG_ERR, "s_connect: connect %s: %s (%d)",
               dst, s_strerror(error), error);
            str_free(dst);
            return -1;
        }
        if(s_poll_canwrite(c->fds, c->fd)) {
            s_log(LOG_NOTICE, "s_connect: connected %s", dst);
            str_free(dst);
            return 0; /* success */
        }
        s_log(LOG_ERR, "s_connect: s_poll_wait %s: internal error",
            dst);
        str_free(dst);
        return -1;
    }
    return -1; /* should not be possible */
}
