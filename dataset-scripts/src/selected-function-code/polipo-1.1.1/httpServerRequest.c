int
httpServerRequest(ObjectPtr object, int method, int from, int to, 
                  HTTPRequestPtr requestor, void *closure)
{
    int rc;
    char name[132];
    int port;
    int x, y, z;

    assert(from >= 0 && (to < 0 || to > from));
    assert(closure == NULL);
    assert(!(object->flags & OBJECT_LOCAL));
    assert(object->type == OBJECT_HTTP);

    if(object->flags & OBJECT_INPROGRESS)
        return 1;

    if(requestor->flags & REQUEST_REQUESTED)
        return 0;

    assert(requestor->request == NULL);

    if(proxyOffline)
        return -1;

    rc = parseUrl(object->key, object->key_size, &x, &y, &port, &z);
    
    if(rc < 0 || x < 0 || y < 0 || y - x > 131) {
        do_log(L_ERROR, "Couldn't parse URL %s\n", scrub(object->key));
        abortObject(object, 400, internAtom("Couldn't parse URL"));
        notifyObject(object);
        return 1;
    }

    if(!intListMember(port, allowedPorts)) {
        do_log(L_ERROR, "Attempted connection to port %d.\n", port);
        abortObject(object, 403, internAtom("Forbidden port"));
        notifyObject(object);
        return 1;
    }

    memcpy(name, ((char*)object->key) + x, y - x);
    name[y - x] = '\0';

    requestor->flags |= REQUEST_REQUESTED;
    rc = httpMakeServerRequest(name, port, object, method, from, to,
                               requestor);
                                   
    if(rc < 0) {
        abortObject(object, 
                    503, internAtom("Couldn't schedule server request"));
        notifyObject(object);
        return 1;
    }

    return 1;
}