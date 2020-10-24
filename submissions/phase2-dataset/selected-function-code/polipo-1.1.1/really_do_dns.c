static int
really_do_dns(AtomPtr name, ObjectPtr object)
{
    int rc;
    DnsQueryPtr query;
    AtomPtr message = NULL;
    int id;
    AtomPtr a = NULL;

    if(a == NULL) {
        if(name == atomLocalhost || name == atomLocalhostDot) {
            char s[1 + sizeof(HostAddressRec)];
            memset(s, 0, sizeof(s));
            s[0] = DNS_A;
            s[1] = 4;
            s[2] = 127;
            s[3] = 0;
            s[4] = 0;
            s[5] = 1;
            a = internAtomN(s, 1 + sizeof(HostAddressRec));
            if(a == NULL) {
                abortObject(object, 501,
                            internAtom("Couldn't allocate address"));
                notifyObject(object);
                errno = ENOMEM;
                return -1;
            }
        }
    }

    if(a == NULL) {
        struct in_addr ina;
        rc = inet_aton(name->string, &ina);
        if(rc == 1) {
            char s[1 + sizeof(HostAddressRec)];
            memset(s, 0, sizeof(s));
            s[0] = DNS_A;
            s[1] = 4;
            memcpy(s + 2, &ina, 4);
            a = internAtomN(s, 1 + sizeof(HostAddressRec));
            if(a == NULL) {
                abortObject(object, 501,
                            internAtom("Couldn't allocate address"));
                notifyObject(object);
                errno = ENOMEM;
                return -1;
            }
        }
    }
#ifdef HAVE_IPv6
    if(a == NULL)
        a = rfc2732(name);
#endif

    if(a) {
        object->headers = a;
        object->age = current_time.tv_sec;
        object->expires = current_time.tv_sec + 240;
        object->flags &= ~(OBJECT_INITIAL | OBJECT_INPROGRESS);
        notifyObject(object);
        return 0;
    }

    rc = establishDnsSocket();
    if(rc < 0) {
        do_log_error(L_ERROR, -rc, "Couldn't establish DNS socket.\n");
        message = internAtomError(-rc, "Couldn't establish DNS socket");
        goto fallback;
    }

    /* The id is used to speed up detecting replies to queries that
       are no longer current -- see dnsReplyHandler. */
    id = (idSeed++) & 0xFFFF;

    query = malloc(sizeof(DnsQueryRec));
    if(query == NULL) {
        do_log(L_ERROR, "Couldn't allocate DNS query.\n");
        message = internAtom("Couldn't allocate DNS query");
        goto fallback;
    }
    query->id = id;
    query->inet4 = NULL;
    query->inet6 = NULL;
    query->name = name;
    query->time = current_time.tv_sec;
    query->object = retainObject(object);
    query->timeout = 4;
    query->timeout_handler = NULL;
    query->next = NULL;

    query->timeout_handler = 
        scheduleTimeEvent(query->timeout, dnsTimeoutHandler,
                          sizeof(query), &query);
    if(query->timeout_handler == NULL) {
        do_log(L_ERROR, "Couldn't schedule DNS timeout handler.\n");
        message = internAtom("Couldn't schedule DNS timeout handler");
        goto free_fallback;
    }
    insertQuery(query);

    object->flags |= OBJECT_INPROGRESS;
    rc = sendQuery(query);
    if(rc < 0) {
        if(rc != -EWOULDBLOCK && rc != -EAGAIN && rc != -ENOBUFS) {
            object->flags &= ~OBJECT_INPROGRESS;
            message = internAtomError(-rc, "Couldn't send DNS query");
            goto remove_fallback;
        }
        /* else let it timeout */
    }
    releaseAtom(message);
    return 1;

 remove_fallback:
    removeQuery(query);
 free_fallback:
    releaseObject(query->object);
    cancelTimeEvent(query->timeout_handler);
    free(query);
 fallback:
    if(dnsUseGethostbyname >= 1) {
        releaseAtom(message);
        do_log(L_WARN, "Falling back on gethostbyname.\n");
        return really_do_gethostbyname(name, object);
    } else {
        abortObject(object, 501, message);
        notifyObject(object);
        return 1;
    }
}