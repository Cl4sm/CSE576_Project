int
do_gethostbyname(char *origname,
                 int count,
                 int (*handler)(int, GethostbynameRequestPtr),
                 void *data)
{
    ObjectPtr object;
    int n = strlen(origname);
    AtomPtr name;
    GethostbynameRequestRec request;
    int done, rc;

    memset(&request, 0, sizeof(request));
    request.name = NULL;
    request.addr = NULL;
    request.error_message = NULL;
    request.count = count;
    request.handler = handler;
    request.data = data;

    if(n <= 0 || n > 131) {
        if(n <= 0) {
            request.error_message = internAtom("empty name");
            do_log(L_ERROR, "Empty DNS name.\n");
            done = handler(-EINVAL, &request);
        } else {
            request.error_message = internAtom("name too long");
            do_log(L_ERROR, "DNS name too long.\n");
            done = handler(-ENAMETOOLONG, &request);
        }
        assert(done);
        releaseAtom(request.error_message);
        return 1;
    }

    if(origname[n - 1] == '.')
        n--;

    name = internAtomLowerN(origname, n);

    if(name == NULL) {
        request.error_message = internAtom("couldn't allocate name");
        do_log(L_ERROR, "Couldn't allocate DNS name.\n");
        done = handler(-ENOMEM, &request);
        assert(done);
        releaseAtom(request.error_message);
        return 1;
    }

    request.name = name;
    request.addr = NULL;
    request.error_message = NULL;
    request.count = count;
    request.object = NULL;
    request.handler = handler;
    request.data = data;

    object = findObject(OBJECT_DNS, name->string, name->length);
    if(object == NULL || objectMustRevalidate(object, NULL)) {
        if(object) {
            privatiseObject(object, 0);
            releaseObject(object);
        }
        object = makeObject(OBJECT_DNS, name->string, name->length, 1, 0,
                            NULL, NULL);
        if(object == NULL) {
            request.error_message = internAtom("Couldn't allocate object");
            do_log(L_ERROR, "Couldn't allocate DNS object.\n");
            done = handler(-ENOMEM, &request);
            assert(done);
            releaseAtom(name);
            releaseAtom(request.error_message);
            return 1;
        }
    }

    if((object->flags & (OBJECT_INITIAL | OBJECT_INPROGRESS)) ==
       OBJECT_INITIAL) {
        if(dnsUseGethostbyname >= 3)
            rc = really_do_gethostbyname(name, object);
        else
            rc = really_do_dns(name, object);
        if(rc < 0) {
            assert(!(object->flags & (OBJECT_INITIAL | OBJECT_INPROGRESS)));
            goto fail;
        }
    }

    if(dnsUseGethostbyname >= 3)
        assert(!(object->flags & OBJECT_INITIAL));

#ifndef NO_FANCY_RESOLVER    
    if(object->flags & OBJECT_INITIAL) {
        ConditionHandlerPtr chandler;
        assert(object->flags & OBJECT_INPROGRESS);
        request.object = object;
        chandler = conditionWait(&object->condition, dnsHandler,
                                 sizeof(request), &request);
        if(chandler == NULL)
            goto fail;
        return 1;
    }
#endif

    if(object->headers && object->headers->length > 0) {
        if(object->headers->string[0] == DNS_A)
            assert(((object->headers->length - 1) % 
                    sizeof(HostAddressRec)) == 0);
        else
            assert(object->headers->string[0] == DNS_CNAME);
        request.addr = retainAtom(object->headers);
    } else if(object->message) {
        request.error_message = retainAtom(object->message);
    }

    releaseObject(object);

    if(request.addr && request.addr->length > 0)
        done = handler(1, &request);
    else
        done = handler(-EDNS_HOST_NOT_FOUND, &request);
    assert(done);

    releaseAtom(request.addr); request.addr = NULL;
    releaseAtom(request.name); request.name = NULL;
    releaseAtom(request.error_message); request.error_message = NULL;
    return 1;

 fail:
    releaseNotifyObject(object);
    done = handler(-errno, &request);
    assert(done);
    releaseAtom(name);
    return 1;
}