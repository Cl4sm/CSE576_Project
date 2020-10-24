void daemon_loop(void) {
    SERVICE_OPTIONS *opt;
    int temporary_lack_of_resources;

    while(1) {
        temporary_lack_of_resources=0;
        if(s_poll_wait(fds, -1, -1)>=0) {
            if(s_poll_canread(fds, signal_pipe[0]))
                if(signal_pipe_dispatch()) /* received SIGNAL_TERMINATE */
                    break; /* terminate daemon_loop */
            for(opt=service_options.next; opt; opt=opt->next)
                if(opt->option.accept && s_poll_canread(fds, opt->fd))
                    if(accept_connection(opt))
                        temporary_lack_of_resources=1;
        } else {
            log_error(LOG_NOTICE, get_last_socket_error(),
                "daemon_loop: s_poll_wait");
            temporary_lack_of_resources=1;
        }
        if(temporary_lack_of_resources) {
            s_log(LOG_NOTICE,
                "Accepting new connections suspended for 1 second");
            sleep(1); /* to avoid log trashing */
        }
    }
}
