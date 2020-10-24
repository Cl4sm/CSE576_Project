int
httpSetTimeout(HTTPConnectionPtr connection, int secs)
{
    TimeEventHandlerPtr new;

    if(connection->timeout)
        cancelTimeEvent(connection->timeout);
    connection->timeout = NULL;

    if(secs > 0) {
        new = scheduleTimeEvent(secs, httpTimeoutHandler,
                                sizeof(connection), &connection);
        if(!new) {
            do_log(L_ERROR, "Couldn't schedule timeout for connection 0x%lx\n",
                   (unsigned long)connection);
            return -1;
        }
    } else {
        new = NULL;
    }

    connection->timeout = new;
    return 1;
}