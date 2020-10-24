static TimeEventHandlerPtr
enqueueTimeEvent(TimeEventHandlerPtr event)
{
    TimeEventHandlerPtr otherevent;

    /* We try to optimise two cases -- the event happens very soon, or
       it happens after most of the other events. */
    if(timeEventQueue == NULL ||
       timeval_cmp(&event->time, &timeEventQueue->time) < 0) {
        /* It's the first event */
        event->next = timeEventQueue;
        event->previous = NULL;
        if(timeEventQueue) {
            timeEventQueue->previous = event;
        } else {
            timeEventQueueLast = event;
        }
        timeEventQueue = event;
    } else if(timeval_cmp(&event->time, &timeEventQueueLast->time) >= 0) {
        /* It's the last one */
        event->next = NULL;
        event->previous = timeEventQueueLast;
        timeEventQueueLast->next = event;
        timeEventQueueLast = event;
    } else {
        /* Walk from the end */
        otherevent = timeEventQueueLast;
        while(otherevent->previous &&
              timeval_cmp(&event->time, &otherevent->previous->time) < 0) {
            otherevent = otherevent->previous;
        }
        event->next = otherevent;
        event->previous = otherevent->previous;
        if(otherevent->previous) {
            otherevent->previous->next = event;
        } else {
            timeEventQueue = event;
        }
        otherevent->previous = event;
    }
    return event;
}