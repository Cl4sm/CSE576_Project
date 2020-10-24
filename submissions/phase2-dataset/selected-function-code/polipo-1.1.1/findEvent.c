static FdEventHandlerPtr
findEvent(int revents, FdEventHandlerPtr events)
{
    FdEventHandlerPtr event;

    assert(!(revents & POLLNVAL));
    
    if((revents & POLLHUP) || (revents & POLLERR)) {
        event = findEventHelper(POLLOUT, events);
        if(event) return event;

        event = findEventHelper(POLLIN, events);
        if(event) return event;
        return NULL;
    }

    if(revents & POLLOUT) {
        event = findEventHelper(POLLOUT, events);
        if(event) return event;
    }

    if(revents & POLLIN) {
        event = findEventHelper(POLLIN, events);
        if(event) return event;
    }
    return NULL;
}