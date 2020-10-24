TimeEventHandlerPtr
scheduleTimeEvent(int seconds,
                  int (*handler)(TimeEventHandlerPtr), int dsize, void *data)
{
    struct timeval when;
    TimeEventHandlerPtr event;

    if(seconds >= 0) {
        when = current_time;
        when.tv_sec += seconds;
    } else {
        when.tv_sec = 0;
        when.tv_usec = 0;
    }

    event = malloc(sizeof(TimeEventHandlerRec) - 1 + dsize);
    if(event == NULL) {
        do_log(L_ERROR, "Couldn't allocate time event handler -- "
               "discarding all objects.\n");
        exitFlag = 2;
        return NULL;
    }

    event->time = when;
    event->handler = handler;
    /* Let the compiler optimise the common case */
    if(dsize == sizeof(void*))
        memcpy(event->data, data, sizeof(void*));
    else if(dsize > 0)
        memcpy(event->data, data, dsize);

    return enqueueTimeEvent(event);
}