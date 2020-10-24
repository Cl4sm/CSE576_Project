NOEXPORT void client_run(CLI *c) {
    int err, rst;
#ifndef USE_FORK
    int num_clients_copy;
#endif

#ifndef USE_FORK
    enter_critical_section(CRIT_CLIENTS);
    ui_clients(++num_clients);
    leave_critical_section(CRIT_CLIENTS);
#endif

    c->remote_fd.fd=-1;
    c->fd=-1;
    c->ssl=NULL;
    c->sock_bytes=c->ssl_bytes=0;
    if(c->opt->option.client) {
        c->sock_rfd=&(c->local_rfd);
        c->sock_wfd=&(c->local_wfd);
        c->ssl_rfd=c->ssl_wfd=&(c->remote_fd);
    } else {
        c->sock_rfd=c->sock_wfd=&(c->remote_fd);
        c->ssl_rfd=&(c->local_rfd);
        c->ssl_wfd=&(c->local_wfd);
    }
    c->fds=s_poll_alloc();
    addrlist_init(&c->connect_addr, 1);

    err=setjmp(c->err);
    if(!err)
        client_try(c);
    rst=err==1 && c->opt->option.reset;
    s_log(LOG_NOTICE,
        "Connection %s: %d byte(s) sent to SSL, %d byte(s) sent to socket",
         rst ? "reset" : "closed", c->ssl_bytes, c->sock_bytes);

        /* cleanup temporary (e.g. IDENT) socket */
    if(c->fd>=0)
        closesocket(c->fd);
    c->fd=-1;

        /* cleanup SSL */
    if(c->ssl) { /* SSL initialized */
        SSL_set_shutdown(c->ssl, SSL_SENT_SHUTDOWN|SSL_RECEIVED_SHUTDOWN);
        SSL_free(c->ssl);
        c->ssl=NULL;
        ERR_remove_state(0);
    }

        /* cleanup remote socket */
    if(c->remote_fd.fd>=0) { /* remote socket initialized */
        if(rst && c->remote_fd.is_socket) /* reset */
            reset(c->remote_fd.fd, "linger (remote)");
        closesocket(c->remote_fd.fd);
        s_log(LOG_DEBUG, "Remote socket (FD=%d) closed", c->remote_fd.fd);
        c->remote_fd.fd=-1;
    }

        /* cleanup local socket */
    if(c->local_rfd.fd>=0) { /* local socket initialized */
        if(c->local_rfd.fd==c->local_wfd.fd) {
            if(rst && c->local_rfd.is_socket)
                reset(c->local_rfd.fd, "linger (local)");
            closesocket(c->local_rfd.fd);
            s_log(LOG_DEBUG, "Local socket (FD=%d) closed", c->local_rfd.fd);
        } else { /* stdin/stdout */
            if(rst && c->local_rfd.is_socket)
                reset(c->local_rfd.fd, "linger (local_rfd)");
            if(rst && c->local_wfd.is_socket)
                reset(c->local_wfd.fd, "linger (local_wfd)");
        }
        c->local_rfd.fd=c->local_wfd.fd=-1;
    }

#ifdef USE_FORK
    /* display child return code if it managed to arrive on time */
    /* otherwise it will be retrieved by the init process and ignored */
    if(c->opt->option.program) /* 'exec' specified */
        child_status(); /* null SIGCHLD handler was used */
    s_log(LOG_DEBUG, "Service [%s] finished", c->opt->servname);
#else
    enter_critical_section(CRIT_CLIENTS);
    ui_clients(--num_clients);
    num_clients_copy=num_clients; /* to move s_log() away from CRIT_CLIENTS */
    leave_critical_section(CRIT_CLIENTS);
    s_log(LOG_DEBUG, "Service [%s] finished (%d left)",
        c->opt->servname, num_clients_copy);
#endif

        /* free remaining memory structures */
    if(c->connect_addr.addr)
        str_free(c->connect_addr.addr);
    s_poll_free(c->fds);
    c->fds=NULL;
}
