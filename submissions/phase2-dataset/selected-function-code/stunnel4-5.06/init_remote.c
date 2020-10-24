    /* where to bind connecting socket */
    if(c->opt->option.local) /* outgoing interface */
        c->bind_addr=&c->opt->source_addr;
#ifndef USE_WIN32
    else if(c->opt->option.transparent_src)
        c->bind_addr=&c->peer_addr;
#endif
    else
        c->bind_addr=NULL; /* don't bind */

    /* setup c->remote_fd, now */
    if(c->opt->option.remote
#ifndef USE_WIN32
                || c->opt->option.transparent_dst
#endif
            ) {
        /* try remote first for exec+connect targets */
        c->remote_fd.fd=connect_remote(c);
    } else if(c->opt->option.program) { /* exec+connect uses local fd */
        c->remote_fd.fd=connect_local(c);
    } else {
        s_log(LOG_ERR, "INTERNAL ERROR: No target for remote socket");
        longjmp(c->err, 1);
    }

    c->remote_fd.is_socket=1; /* always! */
    s_log(LOG_DEBUG, "Remote socket (FD=%d) initialized", c->remote_fd.fd);
    if(set_socket_options(c->remote_fd.fd, 2))
        s_log(LOG_WARNING, "Failed to set remote socket options");
}
