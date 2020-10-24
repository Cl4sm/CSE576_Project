static int
lingeringCloseHandler(int status, FdEventHandlerPtr event)
{
    LingeringClosePtr l = *(LingeringClosePtr*)event->data;
    char buf[17];
    int rc;

    assert(l->handler == event);

    l->handler = NULL;
    if(status && status != -EDOGRACEFUL)
        goto done;

    rc = READ(l->fd, &buf, 17);
    if(rc == 0 || (rc < 0 && errno != EAGAIN && errno != EINTR))
        goto done;

    /* The client is still sending data.  Ignore it in order to let
       TCP's flow control do its work.  The timeout will close the
       connection. */
    return 1;

 done:
    if(l->timeout) {
        cancelTimeEvent(l->timeout);
        l->timeout = NULL;
    }
    CLOSE(l->fd);
    free(l);
    return 1;
}