int
httpMakeServerRequest(char *name, int port, ObjectPtr object, 
                  int method, int from, int to, HTTPRequestPtr requestor)
{
    HTTPServerPtr server;
    HTTPRequestPtr request;
    int rc;

    assert(!(object->flags & OBJECT_INPROGRESS));

    if(parentHost) {
        server = getServer(parentHost->string, parentPort, 1);
    } else {
        server = getServer(name, port, 0);
    }
    if(server == NULL) return -1;

    object->flags |= OBJECT_INPROGRESS;
    object->requestor = requestor;

    request = httpMakeRequest();
    if(!request) {
        do_log(L_ERROR, "Couldn't allocate request.\n");
        return -1;
    }

    /* Because we allocate objects in chunks, we cannot have data that
       doesn't start at a chunk boundary. */
    if(from % CHUNK_SIZE != 0) {
        if(allowUnalignedRangeRequests) {
            objectFillFromDisk(object, from / CHUNK_SIZE * CHUNK_SIZE, 1);
            if(objectHoleSize(object, from - 1) != 0)
                from = from / CHUNK_SIZE * CHUNK_SIZE;
        } else {
            from = from / CHUNK_SIZE * CHUNK_SIZE;
        }
    }

    request->object = retainObject(object);
    request->method = method;
    if(method == METHOD_CONDITIONAL_GET) {
        if(server->lies > 0)
            request->method = METHOD_HEAD;
    }
    request->flags =
        REQUEST_PERSISTENT |
        (expectContinue ? (requestor->flags & REQUEST_WAIT_CONTINUE) : 0);
    request->from = from;
    request->to = to;
    request->request = requestor;
    requestor->request = request;
    request->cache_control = requestor->cache_control;
    request->time0 = null_time;
    request->time1 = null_time;

    rc = httpServerQueueRequest(server, request);
    if(rc < 0) {
        do_log(L_ERROR, "Couldn't queue request.\n");
        request->request = NULL;
        requestor->request = NULL;
        object->flags &= ~(OBJECT_INPROGRESS | OBJECT_VALIDATING);
        releaseNotifyObject(object);
        httpDestroyRequest(request);
        return 1;
    }

    if(request->flags & REQUEST_WAIT_CONTINUE) {
        if(server->version == HTTP_10) {
            httpServerAbortRequest(request, 1,
                                   417, internAtom("Expectation failed"));
            return 1;
        }
    } else if(expectContinue >= 2 && server->version == HTTP_11) {
        if(request->method == METHOD_POST || request->method == METHOD_PUT)
            request->flags |= REQUEST_WAIT_CONTINUE;
    }
        
 again:
    rc = httpServerTrigger(server);
    if(rc < 0) {
        /* We must be very short on memory.  If there are any requests
           queued, we abort one and try again.  If there aren't, we
           give up. */
        do_log(L_ERROR, "Couldn't trigger server -- out of memory?\n");
        if(server->request) {
            httpServerAbortRequest(server->request, 1, 503,
                                   internAtom("Couldn't trigger server"));
            goto again;
        }
    }
    return 1;
}