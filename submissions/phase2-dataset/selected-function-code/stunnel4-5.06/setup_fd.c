NOEXPORT int setup_fd(int fd, int nonblock, char *msg) {
#if !defined USE_NEW_LINUX_API && defined FD_CLOEXEC
    int err;
#endif

    if(fd<0) {
        sockerror(msg);
        return -1;
    }
#ifndef USE_FORK
    if(max_fds && fd>=max_fds) {
        s_log(LOG_ERR, "%s: FD=%d out of range (max %d)",
            msg, fd, max_fds);
        closesocket(fd);
        return -1;
    }
#endif

#ifdef USE_NEW_LINUX_API
    (void)nonblock; /* skip warning about unused parameter */
#else /* set O_NONBLOCK and F_SETFD */
    set_nonblock(fd, nonblock);
#ifdef FD_CLOEXEC
    do {
        err=fcntl(fd, F_SETFD, FD_CLOEXEC);
    } while(err<0 && get_last_socket_error()==S_EINTR);
    if(err<0)
        sockerror("fcntl SETFD"); /* non-critical */
#endif /* FD_CLOEXEC */
#endif /* USE_NEW_LINUX_API */

#ifdef DEBUG_FD_ALLOC
    s_log(LOG_DEBUG, "%s: FD=%d allocated (%sblocking mode)",
        msg, fd, nonblock?"non-":"");
#endif /* DEBUG_FD_ALLOC */

    return fd;
}
