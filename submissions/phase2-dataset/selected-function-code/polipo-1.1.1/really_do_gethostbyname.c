static int
really_do_gethostbyname(AtomPtr name, ObjectPtr object)
{
    struct addrinfo *ai, *entry, hints;
    int rc;
    int error, i;
    char buf[1024];
    AtomPtr a;

    a = rfc2732(name);
    if(a) {
        object->headers = a;
        object->age = current_time.tv_sec;
        object->expires = current_time.tv_sec + 240;
        object->flags &= ~(OBJECT_INITIAL | OBJECT_INPROGRESS);
        notifyObject(object);
        return 0;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_protocol = IPPROTO_TCP;
    if(dnsQueryIPv6 <= 0)
        hints.ai_family = AF_INET;
    else if(dnsQueryIPv6 >= 3)
        hints.ai_family = AF_INET6;

    rc = getaddrinfo(name->string, NULL, &hints, &ai);

    switch(rc) {
    case 0: error = 0; break;
    case EAI_FAMILY:
#ifdef EAI_ADDRFAMILY
    case EAI_ADDRFAMILY:
#endif
    case EAI_SOCKTYPE:
        error = EAFNOSUPPORT; break;
    case EAI_BADFLAGS: error = EINVAL; break;
    case EAI_SERVICE: error = EDNS_NO_RECOVERY; break;
#ifdef EAI_NONAME
    case EAI_NONAME:
#endif
#ifdef EAI_NODATA
    case EAI_NODATA:
#endif
        error = EDNS_NO_ADDRESS; break;
    case EAI_FAIL: error = EDNS_NO_RECOVERY; break;
    case EAI_AGAIN: error = EDNS_TRY_AGAIN; break;
#ifdef EAI_MEMORY
    case EAI_MEMORY: error = ENOMEM; break;
#endif
    case EAI_SYSTEM: error = errno; break;
    default: error = EUNKNOWN;
    }

    if(error == EDNS_NO_ADDRESS) {
        object->headers = NULL;
        object->age = current_time.tv_sec;
        object->expires = current_time.tv_sec + dnsNegativeTtl;
        object->flags &= ~(OBJECT_INITIAL | OBJECT_INPROGRESS);
        notifyObject(object);
        return 0;
    } else if(error) {
        do_log_error(L_ERROR, error, "Getaddrinfo failed");
        object->flags &= ~OBJECT_INPROGRESS;
        abortObject(object, 404,
                    internAtomError(error, "Getaddrinfo failed"));
        notifyObject(object);
        return 0;
    }

    entry = ai;
    buf[0] = DNS_A;
    i = 0;
    while(entry) {
        HostAddressRec host;
        int host_valid = 0;
        if(entry->ai_family == AF_INET && entry->ai_protocol == IPPROTO_TCP) {
            if(dnsQueryIPv6 < 3) {
                host.af = 4;
                memset(host.data, 0, sizeof(host.data));
                memcpy(&host.data,
                       &((struct sockaddr_in*)entry->ai_addr)->sin_addr, 
                       4);
                host_valid = 1;
            }
        } else if(entry->ai_family == AF_INET6 && 
                  entry->ai_protocol == IPPROTO_TCP) {
            if(dnsQueryIPv6 > 0) {
                host.af = 6;
                memset(&host.data, 0, sizeof(host.data));
                memcpy(&host.data,
                       &((struct sockaddr_in6*)entry->ai_addr)->sin6_addr, 
                       16);
                host_valid = 1;
            }
        }
        if(host_valid) {
            if(i >= 1024 / sizeof(HostAddressRec) - 2) {
                do_log(L_ERROR, "Too many addresses for host %s\n", 
                       name->string);
                break;
            }
            memcpy(buf + 1 + i * sizeof(HostAddressRec), 
                   &host, sizeof(HostAddressRec));
            i++;
        }
        entry = entry->ai_next;
    }
    freeaddrinfo(ai);
    if(i == 0) {
        do_log(L_ERROR, "Getaddrinfo returned no useful addresses\n");
        object->flags &= ~OBJECT_INPROGRESS;
        abortObject(object, 404,
                    internAtom("Getaddrinfo returned no useful addresses"));
        notifyObject(object);
        return 0;
    }

    if(1 <= dnsQueryIPv6 && dnsQueryIPv6 <= 2)
        qsort(buf + 1, i, sizeof(HostAddressRec), compare_hostaddr);

    a = internAtomN(buf, 1 + i * sizeof(HostAddressRec));
    if(a == NULL) {
        object->flags &= ~OBJECT_INPROGRESS;
        abortObject(object, 501, internAtom("Couldn't allocate address"));
        notifyObject(object);
        return 0;
    }
    object->headers = a;
    object->age = current_time.tv_sec;
    object->expires = current_time.tv_sec + dnsGethostbynameTtl;
    object->flags &= ~(OBJECT_INITIAL | OBJECT_INPROGRESS);
    notifyObject(object);
    return 0;
}