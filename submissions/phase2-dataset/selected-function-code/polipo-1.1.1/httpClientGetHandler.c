int
httpClientGetHandler(int status, ConditionHandlerPtr chandler)
{
    HTTPRequestPtr request = *(HTTPRequestPtr*)chandler->data;
    HTTPConnectionPtr connection = request->connection;
    ObjectPtr object = request->object;
    int rc;

    assert(request == connection->request);

    if(request->request) {
        assert(request->object->flags & OBJECT_INPROGRESS);
        assert(!request->request->object ||
               request->request->object == request->object);
    }

    if(status < 0) {
        object->flags &= ~OBJECT_VALIDATING; /* for now */
        if(request->request && request->request->request == request)
            httpServerClientReset(request->request);
        lockChunk(object, request->from / CHUNK_SIZE);
        request->chandler = NULL;
        rc = delayedHttpServeObject(connection);
        if(rc < 0) {
            unlockChunk(object, request->from / CHUNK_SIZE);
            do_log(L_ERROR, "Couldn't schedule serving.\n");
            abortObject(object, 503, internAtom("Couldn't schedule serving"));
        }
        return 1;
    }

    if(object->flags & OBJECT_VALIDATING)
        return 0;

    if(request->error_code) {
        lockChunk(object, request->from / CHUNK_SIZE);
        request->chandler = NULL;
        rc = delayedHttpServeObject(connection);
        if(rc < 0) {
            unlockChunk(object, request->from / CHUNK_SIZE);
            do_log(L_ERROR, "Couldn't schedule serving.\n");
            abortObject(object, 503, internAtom("Couldn't schedule serving"));
        }
        return 1;
    }

    if(request->flags & REQUEST_WAIT_CONTINUE) {
        if(request->request && 
           !(request->request->flags & REQUEST_WAIT_CONTINUE)) {
            request->flags &= ~REQUEST_WAIT_CONTINUE;
            delayedHttpClientContinue(connection);
        }
        return 0;
    }

    /* See httpServerHandlerHeaders */
    if((object->flags & OBJECT_SUPERSEDED) &&
       /* Avoid superseding loops. */
       !(request->flags & REQUEST_SUPERSEDED) &&
       request->request && request->request->can_mutate) {
        ObjectPtr new_object = retainObject(request->request->can_mutate);
        if(object->requestor == request) {
            if(new_object->requestor == NULL)
                new_object->requestor = request;
            object->requestor = NULL;
            /* Avoid superseding the same request more than once. */
            request->flags |= REQUEST_SUPERSEDED;
        }
        request->chandler = NULL;
        releaseObject(object);
        request->object = new_object;
        request->request->object = new_object;
        /* We're handling the wrong object now.  It's simpler to
           rebuild the whole data structure from scratch rather than
           trying to compensate. */
        rc = delayedHttpClientNoticeRequest(request);
        if(rc < 0) {
            do_log(L_ERROR, "Couldn't schedule noticing of request.");
            abortObject(object, 500,
                        internAtom("Couldn't schedule "
                                   "noticing of request"));
            /* We're probably out of memory.  What can we do? */
            shutdown(connection->fd, 1);
        }
        return 1;
    }

    if(object->requestor != request && !(object->flags & OBJECT_ABORTED)) {
        /* Make sure we don't serve an object that is stale for us
           unless we're the requestor. */
        if((object->flags & (OBJECT_LINEAR | OBJECT_MUTATING)) ||
           objectMustRevalidate(object, &request->cache_control)) {
           if(object->flags & OBJECT_INPROGRESS)
               return 0;
           rc = delayedHttpClientNoticeRequest(request);
           if(rc < 0) {
               do_log(L_ERROR, "Couldn't schedule noticing of request.");
               abortObject(object, 500,
                           internAtom("Couldn't schedule "
                                      "noticing of request"));
           } else {
               request->chandler = NULL;
               return 1;
           }
        }
    }

    if(object->flags & (OBJECT_INITIAL | OBJECT_VALIDATING)) {
        if(object->flags & (OBJECT_INPROGRESS | OBJECT_VALIDATING)) {
            return 0;
        } else if(object->flags & OBJECT_FAILED) {
            if(request->error_code)
                abortObject(object, 
                            request->error_code, 
                            retainAtom(request->error_message));
            else {
                abortObject(object, 500,
                            internAtom("Error message lost in transit"));
            }
        } else {
            /* The request was pruned by httpServerDiscardRequests */
            if(chandler == request->chandler) {
                int rc;
                request->chandler = NULL;
                rc = delayedHttpClientNoticeRequest(request);
                if(rc < 0)
                    abortObject(object, 500,
                                internAtom("Couldn't allocate "
                                           "delayed notice request"));
                else
                    return 1;
            } else {
                abortObject(object, 500,
                            internAtom("Wrong request pruned -- "
                                       "this shouldn't happen"));
            }
        }
    }

    if(request->object->flags & OBJECT_DYNAMIC) {
        if(objectHoleSize(request->object, 0) == 0) {
            request->from = 0;
            request->to = -1;
        } else {
            /* We really should request again if that is not the case */
        }
    }

    lockChunk(object, request->from / CHUNK_SIZE);
    request->chandler = NULL;
    rc = delayedHttpServeObject(connection);
    if(rc < 0) {
        unlockChunk(object, request->from / CHUNK_SIZE);
        do_log(L_ERROR, "Couldn't schedule serving.\n");
        abortObject(object, 503, internAtom("Couldn't schedule serving"));
    }
    return 1;
}