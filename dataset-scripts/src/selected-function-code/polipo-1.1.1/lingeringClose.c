int
lingeringClose(int fd)
{
    int rc;
    LingeringClosePtr l;

    rc = shutdown(fd, 1);
    if(rc < 0) {
        if(errno != ENOTCONN) {
            do_log_error(L_ERROR, errno, "Shutdown failed");
        } else if(errno == EFAULT || errno == EBADF) {
            abort();
        }
        CLOSE(fd);
        return 1;
    }

    l = malloc(sizeof(LingeringCloseRec));
    if(l == NULL)
        goto fail;
    l->fd = fd;
    l->handler = NULL;
    l->timeout = NULL;

    l->timeout = scheduleTimeEvent(10, lingeringCloseTimeoutHandler,
                                   sizeof(LingeringClosePtr), &l);
    if(l->timeout == NULL) {
        free(l);
        goto fail;
    }

    l->handler = registerFdEvent(fd, POLLIN,
                                 lingeringCloseHandler,
                                 sizeof(LingeringClosePtr), &l);
    if(l->handler == NULL) {
        do_log(L_ERROR, "Couldn't schedule lingering close handler.\n");
        /* But don't close -- the timeout will do its work. */
    }
    return 1;

 fail:
    do_log(L_ERROR, "Couldn't schedule lingering close.\n");
    CLOSE(fd);
    return 1;
}