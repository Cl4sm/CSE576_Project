void
initServer(void)
{
    TimeEventHandlerPtr event;
    servers = NULL;

    if(pmmFirstSize || pmmSize) {
        if(pmmSize == 0) pmmSize = pmmFirstSize;
        if(pmmFirstSize == 0) pmmFirstSize = pmmSize;
        pmmSize = roundSize(pmmSize);
        pmmFirstSize = roundSize(pmmFirstSize);
    }

    if(serverMaxSlots < 1)
        serverMaxSlots = 1;
    if(serverSlots < 1)
        serverSlots = 1;
    if(serverSlots > serverMaxSlots)
        serverSlots = serverMaxSlots;
    if(serverSlots1 < serverSlots)
        serverSlots1 = serverSlots;
    if(serverSlots1 > serverMaxSlots)
        serverSlots1 = serverMaxSlots;

    initParentProxy();

    event = scheduleTimeEvent(serverExpireTime / 60 + 60, expireServersHandler,
                              0, NULL);
    if(event == NULL) {
        do_log(L_ERROR, "Couldn't schedule server expiry.\n");
        exit(1);
    }
}