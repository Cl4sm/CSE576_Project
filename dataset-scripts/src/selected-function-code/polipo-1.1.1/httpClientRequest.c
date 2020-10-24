int
httpClientRequest(HTTPRequestPtr request, AtomPtr url)
{
    HTTPConnectionPtr connection = request->connection;
    int i, rc;
    int body_len, body_te;
    AtomPtr headers;
    CacheControlRec cache_control;
    AtomPtr via, expect, auth;
    HTTPConditionPtr condition;
    HTTPRangeRec range;

    assert(!request->chandler);
    assert(connection->reqbuf);

    i = httpParseHeaders(1, url,
                         connection->reqbuf, connection->reqbegin, request,
                         &headers, &body_len, 
                         &cache_control, &condition, &body_te,
                         NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                         &expect, &range, NULL, NULL, &via, &auth);
    if(i < 0) {
        releaseAtom(url);
        do_log(L_ERROR, "Couldn't parse client headers.\n");
        shutdown(connection->fd, 0);
        request->flags &= ~REQUEST_PERSISTENT;
        connection->flags &= ~CONN_READER;
        httpClientNoticeError(request, 503,
                              internAtom("Couldn't parse client headers"));
        return 1;
    }

    connection->reqbegin = i;

    if(body_len < 0) {
        if(request->method == METHOD_GET || request->method == METHOD_HEAD)
            body_len = 0;
    }
    connection->bodylen = body_len;
    connection->reqte = body_te;

    if(authRealm) {
        AtomPtr message = NULL;
        AtomPtr challenge = NULL;
        int code = checkClientAuth(auth, url, &message, &challenge);
        if(auth) {
            releaseAtom(auth);
            auth = NULL;
        }
        if(expect) {
            releaseAtom(expect);
            expect = NULL;
        }
        if(code) {
            request->flags |= REQUEST_FORCE_ERROR;
            httpClientDiscardBody(connection);
            httpClientNoticeErrorHeaders(request, code, message, challenge);
            return 1;
        }
    }

    if(auth) {
        releaseAtom(auth);
        auth = NULL;
    }

    if(expect) {
        if(expect == atom100Continue && REQUEST_SIDE(request)) {
            request->flags |= REQUEST_WAIT_CONTINUE;
        } else {
            httpClientDiscardBody(connection);
            httpClientNoticeError(request, 417,
                                  internAtom("Expectation failed"));
            releaseAtom(expect);
            return 1;
        }
        releaseAtom(expect);
    }

    request->from = range.from < 0 ? 0 : range.from;
    request->to = range.to;
    request->cache_control = cache_control;
    request->via = via;
    request->headers = headers;
    request->condition = condition;
    request->object = NULL;

    if(connection->serviced > 500)
        request->flags &= ~REQUEST_PERSISTENT;

    if(request->method == METHOD_CONNECT) {
        if(connection->flags & CONN_WRITER) {
            /* For now */
            httpClientDiscardBody(connection);
            httpClientNoticeError(request, 500,
                                  internAtom("Pipelined CONNECT "
                                             "not supported"));
            return 1;
        }
        if(connection->flags & CONN_BIGREQBUF) {
            /* For now */
            httpClientDiscardBody(connection);
            httpClientNoticeError(request, 500,
                                  internAtom("CONNECT over big buffer "
                                             "not supported"));
            return 1;
        }
        connection->flags &= ~CONN_READER;
        do_tunnel(connection->fd, connection->reqbuf, 
                  connection->reqbegin, connection->reqlen, url);
        connection->fd = -1;
        connection->reqbuf = NULL;
        connection->reqlen = 0;
        connection->reqbegin = 0;
        httpClientFinish(connection, 2);
        return 1;
    }

    rc = urlForbidden(url, httpClientRequestContinue, request);
    if(rc < 0) {
        do_log(L_ERROR, "Couldn't schedule httpClientRequestContinue.\n");
        httpClientDiscardBody(connection);
        httpClientNoticeError(request, 500,
                              internAtom("Couldn't schedule "
                                         "httpClientRequestContinue"));
        return 1;
    }
    return 1;
}