    SERVICE_OPTIONS *opt;
#ifdef HAVE_STRUCT_SOCKADDR_UN
    struct stat st; /* buffer for stat */
#endif

    s_poll_init(fds);
    s_poll_add(fds, signal_pipe[0], 1, 0);

    for(opt=service_options.next; opt; opt=opt->next) {
        s_log(LOG_DEBUG, "Closing service [%s]", opt->servname);
        if(opt->option.accept && opt->fd>=0) {
            if(opt->fd<listen_fds_start ||
                    opt->fd>=listen_fds_start+systemd_fds)
                closesocket(opt->fd);
            s_log(LOG_DEBUG, "Service [%s] closed (FD=%d)",
                opt->servname, opt->fd);
            opt->fd=-1;
#ifdef HAVE_STRUCT_SOCKADDR_UN
            if(opt->local_addr.sa.sa_family==AF_UNIX) {
                if(lstat(opt->local_addr.un.sun_path, &st))
                    sockerror(opt->local_addr.un.sun_path);
                else if(!S_ISSOCK(st.st_mode))
                    s_log(LOG_ERR, "Not a socket: %s",
                        opt->local_addr.un.sun_path);
                else if(unlink(opt->local_addr.un.sun_path))
                    sockerror(opt->local_addr.un.sun_path);
                else
                    s_log(LOG_DEBUG, "Socket removed: %s",
                        opt->local_addr.un.sun_path);
            }
#endif
        } else if(opt->option.program && opt->option.remote) {
            /* create exec+connect services             */
            /* FIXME: this is just a crude workaround   */
            /*        is it better to kill the service? */
            opt->option.retry=0;
        }
        /* purge session cache of the old SSL_CTX object */
        /* this workaround won't be needed anymore after */
        /* delayed deallocation calls SSL_CTX_free()     */
        if(opt->ctx)
            SSL_CTX_flush_sessions(opt->ctx,
                (long)time(NULL)+opt->session_timeout+1);
        s_log(LOG_DEBUG, "Service [%s] closed", opt->servname);
    }
}
