int
do_scheduled_accept(int status, FdEventHandlerPtr event)
{
    AcceptRequestPtr request = (AcceptRequestPtr)&event->data;
    int rc, done;
    unsigned len;
    struct sockaddr_in addr;

    if(status) {
        done = request->handler(status, event, request);
        if(done) return done;
    }

    len = sizeof(struct sockaddr_in);

    rc = accept(request->fd, (struct sockaddr*)&addr, &len);

    if(rc >= 0)
        done = request->handler(rc, event, request);
    else
        done = request->handler(-errno, event, request);
    return done;
}