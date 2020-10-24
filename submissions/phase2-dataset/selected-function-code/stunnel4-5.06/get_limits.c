    /* start with current ulimit */
#if defined(HAVE_SYSCONF)
    errno=0;
    max_fds=sysconf(_SC_OPEN_MAX);
    if(errno)
        ioerror("sysconf");
    if(max_fds<0)
        max_fds=0; /* unlimited */
#elif defined(HAVE_GETRLIMIT)
    struct rlimit rlim;

    if(getrlimit(RLIMIT_NOFILE, &rlim)<0) {
        ioerror("getrlimit");
        max_fds=0; /* unlimited */
    } else
        max_fds=rlim.rlim_cur!=RLIM_INFINITY ? rlim.rlim_cur : 0;
#else
    max_fds=0; /* unlimited */
#endif /* HAVE_SYSCONF || HAVE_GETRLIMIT */

#if !defined(USE_WIN32) && !defined(USE_POLL) && !defined(__INNOTEK_LIBC__)
    /* apply FD_SETSIZE if select() is used on Unix */
    if(!max_fds || max_fds>FD_SETSIZE)
        max_fds=FD_SETSIZE; /* start with select() limit */
#endif /* select() on Unix */

    /* stunnel needs at least 16 file desriptors */
    if(max_fds && max_fds<16)
        max_fds=16;

    if(max_fds) {
        max_clients=max_fds>=256 ? max_fds*125/256 : (max_fds-6)/2;
        s_log(LOG_DEBUG, "Clients allowed=%d", max_clients);
    } else {
        max_clients=0;
        s_log(LOG_DEBUG, "No limit detected for the number of clients");
    }
}
