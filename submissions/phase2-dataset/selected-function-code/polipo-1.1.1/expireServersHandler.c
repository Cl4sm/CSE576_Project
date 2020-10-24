static int
expireServersHandler(TimeEventHandlerPtr event)
{
    HTTPServerPtr server, next;
    TimeEventHandlerPtr e;
    server = servers;
    while(server) {
        next = server->next;
        if(httpServerIdle(server) &&
           server->time + serverExpireTime < current_time.tv_sec)
            discardServer(server);
        server = next;
    }
    e = scheduleTimeEvent(serverExpireTime / 60 + 60, 
                          expireServersHandler, 0, NULL);
    if(!e) {
        do_log(L_ERROR, "Couldn't schedule server expiry.\n");
        polipoExit();
    }
    return 1;
}