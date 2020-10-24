    s_log(LOG_DEBUG, "Service [%s] started", c->opt->servname);
    if(c->opt->option.program && c->opt->option.remote) {
            /* exec and connect options specified together
             * -> spawn a local program instead of stdio */
        for(;;) {
            SERVICE_OPTIONS *opt=c->opt;
            memset(c, 0, sizeof(CLI)); /* connect_local needs clean c */
            c->opt=opt;
            if(!setjmp(c->err))
                c->local_rfd.fd=c->local_wfd.fd=connect_local(c);
            else
                break;
            client_run(c);
            if(!c->opt->option.retry)
                break;
            sleep(1); /* FIXME: not a good idea in ucontext threading */
            s_poll_free(c->fds);
            c->fds=NULL;
            str_stats(); /* client thread allocation tracking */
            /* c allocation is detached, so it is safe to call str_stats() */
            if(service_options.next) /* don't str_cleanup in inetd mode */
                str_cleanup();
        }
    } else
        client_run(c);
    str_free(c);
}
