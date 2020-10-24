static int
dnsGethostbynameFallback(int id, AtomPtr message)
{
    DnsQueryPtr query, previous;
    ObjectPtr object;

    if(inFlightDnsQueries == NULL) {
        releaseAtom(message);
        return 1;
    }

    query = NULL;
    if(id < 0 || inFlightDnsQueries->id == id) {
        previous = NULL;
        query = inFlightDnsQueries;
    } else {
        previous = inFlightDnsQueries;
        while(previous->next) {
            if(previous->next->id == id) {
                query = previous->next;
                break;
            }
            previous = previous->next;
        }
        if(!query) {
            previous = NULL;
            query = inFlightDnsQueries;
        }
    }

    if(previous == NULL) {
        inFlightDnsQueries = query->next;
        if(inFlightDnsQueries == NULL)
            inFlightDnsQueriesLast = NULL;
    } else {
        previous->next = query->next;
        if(query->next == NULL)
            inFlightDnsQueriesLast = NULL;
    }

    object = makeObject(OBJECT_DNS, query->name->string, query->name->length,
                        1, 0, NULL, NULL);
    if(!object) {
        do_log(L_ERROR, "Couldn't make DNS object.\n");
        releaseAtom(query->name);
        releaseAtom(message);
        releaseObject(query->object);
        cancelTimeEvent(query->timeout_handler);
        free(query);
        return -1;
    }
    if(dnsUseGethostbyname >= 1) {
        releaseAtom(message);
        do_log(L_WARN, "Falling back to using system resolver.\n");
        really_do_gethostbyname(retainAtom(query->name), object);
    } else {
        releaseAtom(object->message);
        object->message = message;
        object->flags &= ~OBJECT_INPROGRESS;
        releaseNotifyObject(object);
    }
    cancelTimeEvent(query->timeout_handler);
    releaseAtom(query->name);
    if(query->inet4) releaseAtom(query->inet4);
    if(query->inet6) releaseAtom(query->inet6);
    releaseObject(query->object);
    free(query);
    return 1;
}