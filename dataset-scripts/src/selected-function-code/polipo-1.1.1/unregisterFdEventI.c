static void
unregisterFdEventI(FdEventHandlerPtr event, int i)
{
    assert(i < fdEventNum && poll_fds[i].fd == event->fd);

    if(fdEvents[i] == event) {
        assert(!event->previous);
        fdEvents[i] = event->next;
    } else {
        event->previous->next = event->next;
    }

    if(fdEventsLast[i] == event) {
        assert(!event->next);
        fdEventsLast[i] = event->previous;
    } else {
        event->next->previous = event->previous;
    }

    free(event);

    if(fdEvents[i] == NULL) {
        deallocateFdEventNum(i);
    } else {
        poll_fds[i].events = recomputePollEvents(fdEvents[i]) | 
            POLLERR | POLLHUP | POLLNVAL;
    }
}