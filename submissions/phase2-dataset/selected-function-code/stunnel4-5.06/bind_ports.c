    SERVICE_OPTIONS *opt;
    char *local_address;
    int listening_section;

#ifdef USE_LIBWRAP
    /* execute after options_cmdline() to know service_options.next,
     * but as early as possible to avoid leaking file descriptors */
    /* retry on each bind_ports() in case stunnel.conf was reloaded
       without "libwrap = no" */
    libwrap_init();
#endif /* USE_LIBWRAP */

    s_poll_init(fds);
    s_poll_add(fds, signal_pipe[0], 1, 0);

    /* allow clean unbind_ports() even though
       bind_ports() was not fully performed */
    for(opt=service_options.next; opt; opt=opt->next)
        if(opt->option.accept)
            opt->fd=-1;

    listening_section=0;
    for(opt=service_options.next; opt; opt=opt->next) {
        if(opt->option.accept) {
            if(listening_section<systemd_fds) {
                opt->fd=listen_fds_start+listening_section;
                s_log(LOG_DEBUG,
                    "Listening file descriptor received from systemd (FD=%d)",
                    opt->fd);
            } else {
                opt->fd=s_socket(opt->local_addr.sa.sa_family,
                    SOCK_STREAM, 0, 1, "accept socket");
                if(opt->fd<0)
                    return 1;
                s_log(LOG_DEBUG, "Listening file descriptor created (FD=%d)",
                    opt->fd);
            }
            if(set_socket_options(opt->fd, 0)<0) {
                closesocket(opt->fd);
                opt->fd=-1;
                return 1;
            }
            /* local socket can't be unnamed */
            local_address=s_ntop(&opt->local_addr, addr_len(&opt->local_addr));
            /* we don't bind or listen on a socket inherited from systemd */
            if(listening_section>=systemd_fds) {
                if(bind(opt->fd, &opt->local_addr.sa, addr_len(&opt->local_addr))) {
                    s_log(LOG_ERR, "Error binding service [%s] to %s",
                        opt->servname, local_address);
                    sockerror("bind");
                    closesocket(opt->fd);
                    opt->fd=-1;
                    str_free(local_address);
                    return 1;
                }
                if(listen(opt->fd, SOMAXCONN)) {
                    sockerror("listen");
                    closesocket(opt->fd);
                    opt->fd=-1;
                    str_free(local_address);
                    return 1;
                }
            }
            s_poll_add(fds, opt->fd, 1, 0);
            s_log(LOG_DEBUG, "Service [%s] (FD=%d) bound to %s",
                opt->servname, opt->fd, local_address);
            str_free(local_address);
            ++listening_section;
        } else if(opt->option.program && opt->option.remote) {
            /* create exec+connect services */
            /* FIXME: needs to be delayed on reload with opt->option.retry set */
            create_client(-1, -1,
                alloc_client_session(opt, -1, -1), client_thread);
        }
    }
    if(listening_section<systemd_fds) {
        s_log(LOG_ERR,
            "Too many listening file descriptors received from systemd, got %d",
            systemd_fds);
        return 1;
    }
    return 0; /* OK */
}
