static int
dnsReplyHandler(int abort, FdEventHandlerPtr event)
{
    int fd = event->fd;
    char buf[2048];
    int len, rc;
    ObjectPtr object;
    unsigned ttl = 0;
    AtomPtr name, value, message = NULL;
    int id;
    int af;
    DnsQueryPtr query;
    AtomPtr cname = NULL;

    if(abort) {
        dnsSocketHandler = NULL;
        rc = establishDnsSocket();
        if(rc < 0) {
            do_log(L_ERROR, "Couldn't reestablish DNS socket.\n");
            /* At this point, we should abort all in-flight
               DNS requests.  Oh, well, they'll timeout anyway. */
        }
        return 1;
    }

    len = recv(fd, buf, 2048, 0);
    if(len <= 0) {
        if(errno == EINTR || errno == EAGAIN) return 0;
        /* This is where we get ECONNREFUSED for an ICMP port unreachable */
        do_log_error(L_ERROR, errno, "DNS: recv failed");
        dnsGethostbynameFallback(-1, message);
        return 0;
    }

    /* This could be a late reply to a query that timed out and was
       resent, a reply to a query that timed out, or a reply to an
       AAAA query when we already got a CNAME reply to the associated
       A.  We filter such replies straight away, without trying to
       parse them. */
    rc = dnsReplyId(buf, 0, len, &id);
    if(rc < 0) {
        do_log(L_WARN, "Short DNS reply.\n");
        return 0;
    }
    if(!findQuery(id, NULL)) {
        return 0;
    }

    rc = dnsDecodeReply(buf, 0, len, &id, &name, &value, &af, &ttl);
    if(rc < 0) {
        assert(value == NULL);
        /* We only want to fallback on gethostbyname if we received a
           reply that we could not understand.  What about truncated
           replies? */
        if(rc < 0) {
            do_log_error(L_WARN, -rc, "DNS");
            if(dnsUseGethostbyname >= 2 ||
               (dnsUseGethostbyname && 
                (rc != -EDNS_HOST_NOT_FOUND && rc != -EDNS_NO_RECOVERY &&
                 rc != -EDNS_FORMAT))) {
                dnsGethostbynameFallback(id, message);
                return 0;
            } else {
                message = internAtom(pstrerror(-rc));
            }
        } else {
            assert(name != NULL && id >= 0 && af >= 0);
        }
    }

    query = findQuery(id, name);
    if(query == NULL) {
        /* Duplicate id ? */
        releaseAtom(value);
        releaseAtom(name);
        return 0;
    }

    /* We're going to use the information in this reply.  If it was an
       error, construct an empty atom to distinguish it from information
       we're still waiting for. */
    if(value == NULL)
        value = internAtom("");

 again:
    if(af == 4) {
        if(query->inet4 == NULL) {
            query->inet4 = value;
            query->ttl4 = current_time.tv_sec + ttl;
        } else
            releaseAtom(value);
    } else if(af == 6) {
        if(query->inet6 == NULL) {
            query->inet6 = value;
            query->ttl6 = current_time.tv_sec + ttl;
        } else
            releaseAtom(value);
    } else if(af == 0) {
        /* Ignore errors in this case. */
        if(query->inet4 && query->inet4->length == 0) {
            releaseAtom(query->inet4);
            query->inet4 = NULL;
        }
        if(query->inet6 && query->inet6->length == 0) {
            releaseAtom(query->inet6);
            query->inet6 = NULL;
        }
        if(query->inet4 || query->inet6) {
            do_log(L_WARN, "Host %s has both %s and CNAME -- "
                   "ignoring CNAME.\n", scrub(query->name->string),
                   query->inet4 ? "A" : "AAAA");
            releaseAtom(value);
            value = internAtom("");
            af = query->inet4 ? 4 : 6;
            goto again;
        } else {
            cname = value;
        }
    }

    if(rc >= 0 && !cname &&
       ((dnsQueryIPv6 < 3 && query->inet4 == NULL) ||
        (dnsQueryIPv6 > 0 && query->inet6 == NULL)))
        return 0;

    /* This query is complete */

    cancelTimeEvent(query->timeout_handler);
    object = query->object;

    if(object->flags & OBJECT_INITIAL) {
        assert(!object->headers);
        if(cname) {
            assert(query->inet4 == NULL && query->inet6 == NULL);
            object->headers = cname;
            object->expires = current_time.tv_sec + ttl;
        } else if((!query->inet4 || query->inet4->length == 0) &&
                  (!query->inet6 || query->inet6->length == 0)) {
            releaseAtom(query->inet4);
            releaseAtom(query->inet6);
            object->expires = current_time.tv_sec + dnsNegativeTtl;
            abortObject(object, 500, retainAtom(message));
        } else if(!query->inet4 || query->inet4->length == 0) {
            object->headers = query->inet6;
            object->expires = query->ttl6;
            releaseAtom(query->inet4);
        } else if(!query->inet6 || query->inet6->length == 0) {
            object->headers = query->inet4;
            object->expires = query->ttl4;
            releaseAtom(query->inet6);
        } else {
            /* need to merge results */
            char buf[1024];
            if(query->inet4->length + query->inet6->length > 1024) {
                releaseAtom(query->inet4);
                releaseAtom(query->inet6);
                abortObject(object, 500, internAtom("DNS reply too long"));
            } else {
                if(dnsQueryIPv6 <= 1) {
                    memcpy(buf, query->inet4->string, query->inet4->length);
                    memcpy(buf + query->inet4->length,
                           query->inet6->string + 1, query->inet6->length - 1);
                } else {
                    memcpy(buf, query->inet6->string, query->inet6->length);
                    memcpy(buf + query->inet6->length,
                           query->inet4->string + 1, query->inet4->length - 1);
                }
                object->headers =
                    internAtomN(buf, 
                                query->inet4->length + 
                                query->inet6->length - 1);
                if(object->headers == NULL)
                    abortObject(object, 500, 
                                internAtom("Couldn't allocate DNS atom"));
            }
            object->expires = MIN(query->ttl4, query->ttl6);
        }
        object->age = current_time.tv_sec;
        object->flags &= ~(OBJECT_INITIAL | OBJECT_INPROGRESS);
    } else {
        do_log(L_WARN, "DNS object ex nihilo for %s.\n",
               scrub(query->name->string));
    }
    
    removeQuery(query);
    free(query);

    releaseAtom(name);
    releaseAtom(message);
    releaseNotifyObject(object);
    return 0;
}