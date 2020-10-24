NOEXPORT int daemonize(int fd) { /* go to background */
    if(global_options.option.foreground)
        return 0;
    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);
#if defined(HAVE_DAEMON) && !defined(__BEOS__)
    /* set noclose option when calling daemon() function,
     * so it does not require /dev/null device in the chrooted directory */
    if(daemon(0, 1)==-1) {
        ioerror("daemon");
        return 1;
    }
#else
    chdir("/");
    switch(fork()) {
    case -1:    /* fork failed */
        ioerror("fork");
        return 1;
    case 0:     /* child */
        break;
    default:    /* parent */
        exit(0);
    }
#endif
#ifdef HAVE_SETSID
    setsid(); /* ignore the error */
#endif
    return 0;
}
