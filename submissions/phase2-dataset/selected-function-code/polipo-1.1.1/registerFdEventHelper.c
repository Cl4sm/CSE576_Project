FdEventHandlerPtr
registerFdEventHelper(FdEventHandlerPtr event)
{
    int i;
    int fd = event->fd;

    for(i = 0; i < fdEventNum; i++)
        if(poll_fds[i].fd == fd)
            break;

    if(i >= fdEventNum)
        i = allocateFdEventNum(fd);
    if(i < 0) {
        free(event);
        return NULL;
    }

    event->next = NULL;
    event->previous = fdEventsLast[i];
    if(fdEvents[i] == NULL) {
        fdEvents[i] = event;
    } else {
        fdEventsLast[i]->next = event;
    }
    fdEventsLast[i] = event;
    poll_fds[i].events |= event->poll_events;

    return event;
}