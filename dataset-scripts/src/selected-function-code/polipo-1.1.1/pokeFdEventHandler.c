static int
pokeFdEventHandler(TimeEventHandlerPtr tevent)
{
    FdEventHandlerPokePtr poke = (FdEventHandlerPokePtr)tevent->data;
    int fd = poke->fd;
    int what = poke->what;
    int status = poke->status;
    int done;
    FdEventHandlerPtr event, next;
    int i;

    for(i = 0; i < fdEventNum; i++) {
        if(poll_fds[i].fd == fd)
            break;
    }

    if(i >= fdEventNum)
        return 1;

    event = fdEvents[i];
    while(event) {
        next = event->next;
        if(event->poll_events & what) {
            done = event->handler(status, event);
            if(done) {
                if(fds_invalid)
                    unregisterFdEvent(event);
                else
                    unregisterFdEventI(event, i);
            }
            if(fds_invalid)
                break;
        }
        event = next;
    }
    return 1;
}