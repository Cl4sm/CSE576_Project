int
httpClientRequestContinue(int forbidden_code, AtomPtr url,
                          AtomPtr forbidden_message, AtomPtr forbidden_headers,
                          void *closure)
{
    HTTPRequestPtr request = (HTTPRequestPtr)closure;
    HTTPConnectionPtr connection = request->connection;
    RequestFunction requestfn;
    ObjectPtr object = NULL;

    if(forbidden_code < 0) {
        releaseAtom(url);
        httpClientDiscardBody(connection);
        httpClientNoticeError(request, 500, 
                              internAtomError(-forbidden_code,
                                              "Couldn't test for forbidden "
                                              "URL"));
        return 1;
    }

    if(forbidden_code) {
        releaseAtom(url);
        httpClientDiscardBody(connection);
        httpClientNoticeErrorHeaders(request,
                                     forbidden_code, forbidden_message,
                                     forbidden_headers);
        return 1;
    }

    requestfn = 
        urlIsLocal(url->string, url->length) ? 
        httpLocalRequest :
        httpServerRequest;

    if(request->method == METHOD_POST || request->method == METHOD_PUT) {
        do {
            object = findObject(OBJECT_HTTP, url->string, url->length);
            if(object) {
                privatiseObject(object, 0);
                releaseObject(object);
            }
        } while(object);
        request->object = makeObject(OBJECT_HTTP, url->string, url->length,
                                     0, 0, requestfn, NULL);
        if(request->object == NULL) {
            httpClientDiscardBody(connection);
            httpClientNoticeError(request, 503,
                                  internAtom("Couldn't allocate object"));
            return 1;
        }
        if(requestfn == httpLocalRequest)
            request->object->flags |= OBJECT_LOCAL;
        return httpClientSideRequest(request);
    }

    if(request->cache_control.flags & CACHE_AUTHORIZATION) {
        do {
            object = makeObject(OBJECT_HTTP, url->string, url->length, 0, 0,
                                requestfn, NULL);
            if(object && object->flags != OBJECT_INITIAL) {
                if(!(object->cache_control & CACHE_PUBLIC)) {
                    privatiseObject(object, 0);
                    releaseObject(object);
                    object = NULL;
                } else
                    break;
            }
        } while(object == NULL);
        if(object)
            object->flags |= OBJECT_LINEAR;
    } else {
        object = findObject(OBJECT_HTTP, url->string, url->length);
        if(!object)
            object = makeObject(OBJECT_HTTP, url->string, url->length, 1, 1,
                                requestfn, NULL);
    }
    releaseAtom(url);
    url = NULL;

    if(!object) {
        do_log(L_ERROR, "Couldn't allocate object.\n");
        httpClientDiscardBody(connection);
        httpClientNoticeError(request, 503,
                              internAtom("Couldn't allocate object"));
        return 1;
    }

    if(object->request == httpLocalRequest) {
        object->flags |= OBJECT_LOCAL;
    } else {
        if(disableProxy) {
            httpClientDiscardBody(connection);
            httpClientNoticeError(request, 403,
                                  internAtom("Proxying disabled"));
            releaseObject(object);
            return 1;
        }

        if(!checkVia(proxyName, request->via)) {
            httpClientDiscardBody(connection);
            httpClientNoticeError(request, 504, 
                                  internAtom("Proxy loop detected"));
            releaseObject(object);
            return 1;
        }
    }

    request->object = object;

    httpClientDiscardBody(connection);
    httpClientNoticeRequest(request, 0);
    return 1;
}