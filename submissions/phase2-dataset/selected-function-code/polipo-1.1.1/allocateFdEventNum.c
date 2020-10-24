int
allocateFdEventNum(int fd)
{
    int i;
    if(fdEventNum < fdEventSize) {
        i = fdEventNum;
        fdEventNum++;
    } else {
        struct pollfd *new_poll_fds;
        FdEventHandlerPtr *new_fdEvents, *new_fdEventsLast;
        int new_size = 3 * fdEventSize / 2 + 1;

        new_poll_fds = realloc(poll_fds, new_size * sizeof(struct pollfd));
        if(!new_poll_fds)
            return -1;
        new_fdEvents = realloc(fdEvents, new_size * sizeof(FdEventHandlerPtr));
        if(!new_fdEvents)
            return -1;
        new_fdEventsLast = realloc(fdEventsLast, 
                                   new_size * sizeof(FdEventHandlerPtr));
        if(!new_fdEventsLast)
            return -1;

        poll_fds = new_poll_fds;
        fdEvents = new_fdEvents;
        fdEventsLast = new_fdEventsLast;
        fdEventSize = new_size;
        i = fdEventNum;
        fdEventNum++;
    }

    poll_fds[i].fd = fd;
    poll_fds[i].events = POLLERR | POLLHUP | POLLNVAL;
    poll_fds[i].revents = 0;
    fdEvents[i] = NULL;
    fdEventsLast[i] = NULL;
    fds_invalid = 1;
    return i;
}