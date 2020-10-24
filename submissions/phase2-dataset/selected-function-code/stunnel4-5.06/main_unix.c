NOEXPORT int main_unix(int argc, char* argv[]) {
#if !defined(__vms) && !defined(USE_OS2)
    int fd;

    fd=open("/dev/null", O_RDWR); /* open /dev/null before chroot */
    if(fd<0)
        fatal("Could not open /dev/null");
#endif
    main_init();
    if(main_configure(argc>1 ? argv[1] : NULL, argc>2 ? argv[2] : NULL)) {
        close(fd);
        return 1;
    }
    if(service_options.next) { /* there are service sections -> daemon mode */
#if !defined(__vms) && !defined(USE_OS2)
        if(daemonize(fd)) {
            close(fd);
            return 1;
        }
        close(fd);
        /* create_pid() must be called after drop_privileges()
         * or it won't be possible to remove the file on exit */
        /* create_pid() must be called after daemonize()
         * since the final pid is not known beforehand */
        if(create_pid())
            return 1;
#endif
#ifndef USE_OS2
        signal(SIGCHLD, signal_handler); /* handle dead children */
        signal(SIGHUP, signal_handler); /* configuration reload */
        signal(SIGUSR1, signal_handler); /* log reopen */
        signal(SIGPIPE, SIG_IGN); /* ignore broken pipe */
        if(signal(SIGTERM, SIG_IGN)!=SIG_IGN)
            signal(SIGTERM, signal_handler); /* fatal */
        if(signal(SIGQUIT, SIG_IGN)!=SIG_IGN)
            signal(SIGQUIT, signal_handler); /* fatal */
        if(signal(SIGINT, SIG_IGN)!=SIG_IGN)
            signal(SIGINT, signal_handler); /* fatal */
#endif
        daemon_loop();
    } else { /* inetd mode */
#if !defined(__vms) && !defined(USE_OS2)
        close(fd);
#endif /* standard Unix */
#ifndef USE_OS2
        signal(SIGCHLD, SIG_IGN); /* ignore dead children */
        signal(SIGPIPE, SIG_IGN); /* ignore broken pipe */
#endif
        client_main(alloc_client_session(&service_options, 0, 1));
    }
    return 0;
}
