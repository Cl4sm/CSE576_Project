static int
httpClientDelayedShutdown(HTTPConnectionPtr connection)
{
    TimeEventHandlerPtr handler;

    assert(connection->flags & CONN_READER);
    handler = scheduleTimeEvent(1, httpClientDelayedShutdownHandler,
                                sizeof(connection), &connection);
    if(!handler) {
        do_log(L_ERROR, 
               "Couldn't schedule delayed shutdown -- freeing memory.");
        free_chunk_arenas();
        handler = scheduleTimeEvent(1, httpClientDelayedShutdownHandler,
                                    sizeof(connection), &connection);
        if(!handler) {
            do_log(L_ERROR, 
                   "Couldn't schedule delayed shutdown -- aborting.\n");
            polipoExit();
        }
    }
    return 1;
}