NOEXPORT int signal_pipe_dispatch(void) {
    int sig;

    s_log(LOG_DEBUG, "Dispatching signals from the signal pipe");
    while(readsocket(signal_pipe[0], (char *)&sig, sizeof sig)==sizeof sig) {
        switch(sig) {
#ifndef USE_WIN32
        case SIGCHLD:
            s_log(LOG_DEBUG, "Processing SIGCHLD");
#ifdef USE_FORK
            client_status(); /* report status of client process */
#else /* USE_UCONTEXT || USE_PTHREAD */
            child_status();  /* report status of libwrap or 'exec' process */
#endif /* defined USE_FORK */
            break;
#endif /* !defind USE_WIN32 */
        case SIGNAL_RELOAD_CONFIG:
            s_log(LOG_DEBUG, "Processing SIGNAL_RELOAD_CONFIG");
            if(options_parse(CONF_RELOAD)) {
                s_log(LOG_ERR, "Failed to reload the configuration file");
            } else {
                unbind_ports();
                log_close();
                options_apply();
                log_open();
                ui_config_reloaded();
                if(bind_ports()) {
                    /* FIXME: handle the error */
                }
            }
            break;
        case SIGNAL_REOPEN_LOG:
            s_log(LOG_DEBUG, "Processing SIGNAL_REOPEN_LOG");
            log_close();
            log_open();
            s_log(LOG_NOTICE, "Log file reopened");
            break;
        case SIGNAL_TERMINATE:
            s_log(LOG_DEBUG, "Processing SIGNAL_TERMINATE");
            s_log(LOG_NOTICE, "Terminated");
            return 2;
        default:
            s_log(LOG_ERR, "Received signal %d; terminating", sig);
            return 1;
        }
    }
    s_log(LOG_DEBUG, "Signal pipe is empty");
    return 0;
}
