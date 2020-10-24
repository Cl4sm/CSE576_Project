void set_nonblock(int fd, unsigned long nonblock) {
#if defined F_GETFL && defined F_SETFL && defined O_NONBLOCK && !defined __INNOTEK_LIBC__
    int err, flags;

    do {
        flags=fcntl(fd, F_GETFL, 0);
    } while(flags<0 && get_last_socket_error()==S_EINTR);
    if(flags<0) {
        sockerror("fcntl GETFL"); /* non-critical */
        return;
    }
    if(nonblock)
        flags|=O_NONBLOCK;
    else
        flags&=~O_NONBLOCK;
    do {
        err=fcntl(fd, F_SETFL, flags);
    } while(err<0 && get_last_socket_error()==S_EINTR);
    if(err<0)
        sockerror("fcntl SETFL"); /* non-critical */
#else /* WIN32 or similar */
    if(ioctlsocket(fd, FIONBIO, &nonblock)<0)
        sockerror("ioctlsocket"); /* non-critical */
#if 0
    else
        s_log(LOG_DEBUG, "Socket %d set to %s mode",
            fd, nonblock ? "non-blocking" : "blocking");
#endif
#endif
}
