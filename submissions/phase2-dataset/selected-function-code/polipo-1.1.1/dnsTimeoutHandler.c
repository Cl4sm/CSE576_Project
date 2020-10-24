static int
dnsTimeoutHandler(TimeEventHandlerPtr event)
{
    DnsQueryPtr query = *(DnsQueryPtr*)event->data;
    ObjectPtr object = query->object;
    int rc;

    /* People are reporting that this does happen.  And I have no idea why. */
    if(!queryInFlight(query)) {
        do_log(L_ERROR, "BUG: timing out martian query (%s, flags: 0x%x).\n",
               scrub(query->name->string), (unsigned)object->flags);
        return 1;
    }

    query->timeout = MAX(10, query->timeout * 2);

    if(query->timeout > dnsMaxTimeout) {
        abortObject(object, 501, internAtom("Timeout"));
        goto fail;
    } else {
        rc = sendQuery(query);
        if(rc < 0) {
            if(rc != -EWOULDBLOCK && rc != -EAGAIN && rc != -ENOBUFS) {
                abortObject(object, 501,
                            internAtomError(-rc,
                                            "Couldn't send DNS query"));
                goto fail;
            }
            /* else let it timeout */
        }
        query->timeout_handler =
            scheduleTimeEvent(query->timeout, dnsTimeoutHandler,
                              sizeof(query), &query);
        if(query->timeout_handler == NULL) {
            do_log(L_ERROR, "Couldn't schedule DNS timeout handler.\n");
            abortObject(object, 501,
                        internAtom("Couldn't schedule DNS timeout handler"));
            goto fail;
        }
        return 1;
    }

 fail:
    removeQuery(query);
    object->flags &= ~OBJECT_INPROGRESS;
    if(query->inet4) releaseAtom(query->inet4);
    if(query->inet6) releaseAtom(query->inet6);
    free(query);
    releaseNotifyObject(object);
    return 1;
}