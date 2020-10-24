NOEXPORT void init_ssl(CLI *c) {
    int i, err;
    SSL_SESSION *old_session;
    int unsafe_openssl;

    c->ssl=SSL_new(c->opt->ctx);
    if(!c->ssl) {
        sslerror("SSL_new");
        longjmp(c->err, 1);
    }
    SSL_set_ex_data(c->ssl, cli_index, c); /* for callbacks */
    if(c->opt->option.client) {
#ifndef OPENSSL_NO_TLSEXT
        if(c->opt->sni) {
            s_log(LOG_INFO, "SNI: sending servername: %s", c->opt->sni);
            if(!SSL_set_tlsext_host_name(c->ssl, c->opt->sni)) {
                sslerror("SSL_set_tlsext_host_name");
                longjmp(c->err, 1);
            }
        }
#endif
        if(c->opt->session) {
            enter_critical_section(CRIT_SESSION);
            SSL_set_session(c->ssl, c->opt->session);
            leave_critical_section(CRIT_SESSION);
        }
        SSL_set_fd(c->ssl, c->remote_fd.fd);
        SSL_set_connect_state(c->ssl);
    } else { /* SSL server */
        if(c->local_rfd.fd==c->local_wfd.fd)
            SSL_set_fd(c->ssl, c->local_rfd.fd);
        else {
           /* does it make sense to have SSL on STDIN/STDOUT? */
            SSL_set_rfd(c->ssl, c->local_rfd.fd);
            SSL_set_wfd(c->ssl, c->local_wfd.fd);
        }
        SSL_set_accept_state(c->ssl);
    }

    unsafe_openssl=SSLeay()<0x0090810fL ||
        (SSLeay()>=0x10000000L && SSLeay()<0x1000002fL);
    while(1) {
        /* critical section for OpenSSL version < 0.9.8p or 1.x.x < 1.0.0b *
         * this critical section is a crude workaround for CVE-2010-3864   *
         * see http://www.securityfocus.com/bid/44884 for details          *
         * alternative solution is to disable internal session caching     *
         * NOTE: this critical section also covers callbacks (e.g. OCSP)   */
        if(unsafe_openssl)
            enter_critical_section(CRIT_SSL);

        if(c->opt->option.client)
            i=SSL_connect(c->ssl);
        else
            i=SSL_accept(c->ssl);

        if(unsafe_openssl)
            leave_critical_section(CRIT_SSL);

        err=SSL_get_error(c->ssl, i);
        if(err==SSL_ERROR_NONE)
            break; /* ok -> done */
        if(err==SSL_ERROR_WANT_READ || err==SSL_ERROR_WANT_WRITE) {
            s_poll_init(c->fds);
            s_poll_add(c->fds, c->ssl_rfd->fd,
                err==SSL_ERROR_WANT_READ,
                err==SSL_ERROR_WANT_WRITE);
            switch(s_poll_wait(c->fds, c->opt->timeout_busy, 0)) {
            case -1:
                sockerror("init_ssl: s_poll_wait");
                longjmp(c->err, 1);
            case 0:
                s_log(LOG_INFO, "init_ssl: s_poll_wait:"
                    " TIMEOUTbusy exceeded: sending reset");
                longjmp(c->err, 1);
            case 1:
                break; /* OK */
            default:
                s_log(LOG_ERR, "init_ssl: s_poll_wait: unknown result");
                longjmp(c->err, 1);
            }
            continue; /* ok -> retry */
        }
        if(err==SSL_ERROR_SYSCALL) {
            switch(get_last_socket_error()) {
            case S_EINTR:
            case S_EWOULDBLOCK:
#if S_EAGAIN!=S_EWOULDBLOCK
            case S_EAGAIN:
#endif
                continue;
            }
        }
        if(c->opt->option.client)
            sslerror("SSL_connect");
        else
            sslerror("SSL_accept");
        longjmp(c->err, 1);
    }
    s_log(LOG_INFO, "SSL %s: %s",
        c->opt->option.client ? "connected" : "accepted",
        SSL_session_reused(c->ssl) ?
            "previous session reused" : "new session negotiated");
    if(SSL_session_reused(c->ssl)) {
        c->redirect=(uintptr_t)SSL_SESSION_get_ex_data(SSL_get_session(c->ssl),
            redirect_index);
        if(c->opt->redirect_addr.names && !c->redirect) {
            s_log(LOG_ERR, "No application data found in the reused session");
            longjmp(c->err, 1);
        }
    } else { /* a new session was negotiated */
        new_chain(c);
        SSL_SESSION_set_ex_data(SSL_get_session(c->ssl),
            redirect_index, (void *)c->redirect);
        if(c->opt->option.client) {
            enter_critical_section(CRIT_SESSION);
            old_session=c->opt->session;
            c->opt->session=SSL_get1_session(c->ssl); /* store it */
            if(old_session)
                SSL_SESSION_free(old_session); /* release the old one */
            leave_critical_section(CRIT_SESSION);
        } else { /* SSL server */
            SSL_CTX_add_session(c->opt->ctx, SSL_get_session(c->ssl));
        }
        print_cipher(c);
    }
}
