FdEventHandlerPtr 
makeFdEvent(int fd, int poll_events, 
            int (*handler)(int, FdEventHandlerPtr), int dsize, void *data)
{
    FdEventHandlerPtr event;

    event = malloc(sizeof(FdEventHandlerRec) - 1 + dsize);
    if(event == NULL) {
        do_log(L_ERROR, "Couldn't allocate fd event handler -- "
               "discarding all objects.\n");
        exitFlag = 2;
        return NULL;
    }
    event->fd = fd;
    event->poll_events = poll_events;
    event->handler = handler;
    /* Let the compiler optimise the common cases */
    if(dsize == sizeof(void*))
        memcpy(event->data, data, sizeof(void*));
    else if(dsize == sizeof(StreamRequestRec))
        memcpy(event->data, data, sizeof(StreamRequestRec));
    else if(dsize > 0)
        memcpy(event->data, data, dsize);
    return event;
}