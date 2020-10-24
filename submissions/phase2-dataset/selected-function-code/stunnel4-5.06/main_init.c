void main_init() { /* one-time initialization */
#ifdef HAVE_SYSTEMD_SD_DAEMON_H
    systemd_fds=sd_listen_fds(1);
    if(systemd_fds<0)
        fatal("systemd initialization failed");
    listen_fds_start=SD_LISTEN_FDS_START;
#else
    systemd_fds=0; /* no descriptors received */
    listen_fds_start=3; /* the value is not really important */
#endif
    /* basic initialization contains essential functions required for logging
     * subsystem to function properly, thus all errors here are fatal */
    if(ssl_init()) /* initialize SSL library */
        fatal("SSL initialization failed");
    if(sthreads_init()) /* initialize critical sections & SSL callbacks */
        fatal("Threads initialization failed");
    options_defaults();
    options_apply();
#ifndef USE_FORK
    get_limits(); /* required by setup_fd() */
#endif
    fds=s_poll_alloc();
    if(signal_pipe_init())
        fatal("Signal pipe initialization failed: "
            "check your personal firewall");
    stunnel_info(LOG_NOTICE);
}
