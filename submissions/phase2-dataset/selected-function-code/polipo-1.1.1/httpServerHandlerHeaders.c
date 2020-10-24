int
httpServerHandlerHeaders(int eof,
                         FdEventHandlerPtr event,
                         StreamRequestPtr srequest, 
                         HTTPConnectionPtr connection)
{
    HTTPRequestPtr request = connection->request;
    ObjectPtr object = request->object;
    int rc;
    int code, version;
    int full_len;
    AtomPtr headers;
    int len;
    int te;
    CacheControlRec cache_control;
    int age = -1;
    time_t date, last_modified, expires;
    struct timeval *init_time;
    char *etag;
    AtomPtr via, new_via;
    int expect_body;
    HTTPRangeRec content_range;
    ObjectPtr new_object = NULL, old_object = NULL;
    int supersede = 0;
    AtomPtr message = NULL;
    int suspectDynamic;
    AtomPtr url = NULL;

    assert(request->object->flags & OBJECT_INPROGRESS);
    assert(eof >= 0);

    httpSetTimeout(connection, -1);

    if(request->flags & REQUEST_WAIT_CONTINUE) {
        do_log(D_SERVER_CONN, "W   %s:%d.\n",
               connection->server->name, connection->server->port);
        request->flags &= ~REQUEST_WAIT_CONTINUE;
    }

    rc = httpParseServerFirstLine(connection->buf, &code, &version, &message);
    if(rc <= 0) {
        do_log(L_ERROR, "Couldn't parse server status line.\n");
        httpServerAbort(connection, 1, 502,
                        internAtom("Couldn't parse server status line"));
        return 1;
    }

    do_log(D_SERVER_REQ, "Server status: ");
    do_log_n(D_SERVER_REQ, connection->buf, 
             connection->buf[rc - 1] == '\r' ? rc - 2 : rc - 2);
    do_log(D_SERVER_REQ, " (0x%lx for 0x%lx)\n",
           (unsigned long)connection, (unsigned long)object);

    if(version != HTTP_10 && version != HTTP_11) {
        do_log(L_ERROR, "Unknown server HTTP version\n");
        httpServerAbort(connection, 1, 502,
                        internAtom("Unknown server HTTP version"));
        releaseAtom(message);
        return 1;
    } 

    connection->version = version;
    connection->server->version = version;
    request->flags |= REQUEST_PERSISTENT;

    url = internAtomN(object->key, object->key_size);    
    rc = httpParseHeaders(0, url, connection->buf, rc, request,
                          &headers, &len, &cache_control, NULL, &te,
                          &date, &last_modified, &expires, NULL, NULL, NULL,
                          &age, &etag, NULL, NULL, &content_range,
                          NULL, &via, NULL);
    if(rc < 0) {
        do_log(L_ERROR, "Couldn't parse server headers\n");
        releaseAtom(url);
        releaseAtom(message);
        httpServerAbort(connection, 1, 502, 
                        internAtom("Couldn't parse server headers"));
        return 1;
    }

    if(date < 0)
        date = current_time.tv_sec;

    if(code == 100) {
        releaseAtom(url);
        releaseAtom(message);
        /* We've already reset wait_continue above, but we must still
           ensure that the writer notices. */
        notifyObject(request->object);
        connection->len -= rc;
        if(connection->len > 0)
            memmove(connection->buf, connection->buf + rc, connection->len);
        httpServerReply(connection, 1);
        return 1;
    }

    if(code == 101) {
        httpServerAbort(connection, 1, 501,
                        internAtom("Upgrade not implemented"));
        goto fail;
    }

    if(via && !checkVia(proxyName, via)) {
        httpServerAbort(connection, 1, 504, internAtom("Proxy loop detected"));
        goto fail;
    }
    full_len = content_range.full_length;

    if(code == 206) {
        if(content_range.from == -1 || content_range.to == -1) {
            do_log(L_ERROR, "Partial content without range.\n");
            httpServerAbort(connection, 1, 502,
                            internAtom("Partial content without range"));
            goto fail;
        }
        if(len >= 0 && len != content_range.to - content_range.from) {
            do_log(L_ERROR, "Inconsistent partial content.\n");
            httpServerAbort(connection, 1, 502,
                            internAtom("Inconsistent partial content"));
            goto fail;
        }
    } else if(code < 400 && 
              (content_range.from >= 0 || content_range.to >= 0 || 
               content_range.full_length >= 0)) {
        do_log(L_WARN, "Range without partial content.\n");
        /* Damn anakata. */
        content_range.from = -1;
        content_range.to = -1;
        content_range.full_length = -1;
    } else if(code != 304 && code != 412) {
        full_len = len;
    }

    if(te != TE_IDENTITY && te != TE_CHUNKED) {
        do_log(L_ERROR, "Unsupported transfer-encoding\n");
        httpServerAbort(connection, 1, 502,
                        internAtom("Unsupported transfer-encoding"));
        goto fail;
    }

    if(code == 304) {
        if(request->method != METHOD_CONDITIONAL_GET) {
            do_log(L_ERROR, "Unexpected \"not changed\" reply from server\n");
            httpServerAbort(connection, 1, 502,
                            internAtom("Unexpected \"not changed\" "
                                       "reply from server"));
            goto fail;
        }
        if(object->etag && !etag) {
            /* RFC 2616 10.3.5.  Violated by some front-end proxies. */
            do_log(L_WARN, "\"Not changed\" reply with no ETag.\n");
        } 
    }

    if(code == 412) {
        if(request->method != METHOD_CONDITIONAL_GET ||
           (!object->etag && !object->last_modified)) {
            do_log(L_ERROR, 
                   "Unexpected \"precondition failed\" reply from server.\n");
            httpServerAbort(connection, 1, 502,
                            internAtom("Unexpected \"precondition failed\" "
                                       "reply from server"));
            goto fail;
        }
    }

    releaseAtom(url);

    /* Okay, we're going to accept this reply. */

    if((code == 200 || code == 206 || code == 304 || code == 412) &&
       (cache_control.flags & (CACHE_NO | CACHE_NO_STORE) ||
        cache_control.max_age == 0 ||
        (cacheIsShared && cache_control.s_maxage == 0) ||
        (expires >= 0 && expires <= object->age))) {
        do_log(L_UNCACHEABLE, "Uncacheable object %s (%d)\n",
               scrub(object->key), cache_control.flags);
    }

    if(request->time0.tv_sec != null_time.tv_sec)
        init_time = &request->time0;
    else
        init_time = &current_time;
    age = MIN(init_time->tv_sec - age, init_time->tv_sec);

    if(request->method == METHOD_HEAD || 
       code < 200 || code == 204 || code == 304)
        expect_body = 0;
    else if(te == TE_IDENTITY)
        expect_body = (len != 0);
    else
        expect_body = 1;

    connection->chunk_remaining = -1;
    connection->te = te;

    old_object = object;

    connection->server->lies--;

    if(object->cache_control & CACHE_MISMATCH)
        supersede = 1;

    if(code == 304 || code == 412) {
        if((object->etag && etag && strcmp(object->etag, etag) != 0) ||
           (object->last_modified >= 0 && last_modified >= 0 &&
            object->last_modified != last_modified)) {
            do_log(L_ERROR, "Inconsistent \"%s\" reply for %s\n",
                   code == 304 ? "not changed":"precondition failed",
                   scrub(object->key));
            object->flags |= OBJECT_DYNAMIC;
            supersede = 1;
        }
    } else if(!(object->flags & OBJECT_INITIAL)) {
        if((object->last_modified < 0 || last_modified < 0) &&
           (!object->etag || !etag))
            supersede = 1;
        else if(object->last_modified != last_modified)
            supersede = 1;
        else if(object->etag || etag) {
            /* We need to be permissive here so as to deal with some
               front-end proxies that discard ETags on partial
               replies but not on full replies. */
            if(etag && object->etag && strcmp(object->etag, etag) != 0)
                supersede = 1;
            else if(!object->etag)
                supersede = 1;
        }

        if(!supersede && (object->cache_control & CACHE_VARY) &&
           dontTrustVaryETag >= 1) {
            /* Check content-type to work around mod_gzip bugs */
            if(!httpHeaderMatch(atomContentType, object->headers, headers) ||
               !httpHeaderMatch(atomContentEncoding, object->headers, headers))
                supersede = 1;
        }

        if(full_len < 0 && te == TE_IDENTITY) {
            /* It's an HTTP/1.0 CGI.  Be afraid. */
            if(expect_body && content_range.from < 0 && content_range.to < 0)
                supersede = 1;
        }

        if(!supersede && object->length >= 0 && full_len >= 0 &&
                object->length != full_len) {
            do_log(L_WARN, "Inconsistent length.\n");
            supersede = 1;
        }

        if(!supersede &&
           ((object->last_modified >= 0 && last_modified >= 0) ||
            (object->etag && etag))) {
            if(request->method == METHOD_CONDITIONAL_GET) {
                do_log(L_WARN, "Server ignored conditional request.\n");
                connection->server->lies += 10;
                /* Drop the connection? */
            }
        }
    } else if(code == 416) {
        do_log(L_ERROR, "Unexpected \"range not satisfiable\" reply\n");
        httpServerAbort(connection, 1, 502,
                        internAtom("Unexpected \"range not satisfiable\" "
                                   "reply"));
        /* The object may be superseded.  Make sure the next request
           won't be partial. */
        abortObject(object, 502, 
                    internAtom("Unexpected \"range not satisfiable\" reply"));
        return 1;
    }

    if(object->flags & OBJECT_INITIAL)
        supersede = 0;

    if(supersede) {
        do_log(L_SUPERSEDED,
               "Superseding object %s (%d %d %d %s -> %d %d %d %s)\n",
               scrub(old_object->key),
               object->code, object->length, (int)object->last_modified,
               object->etag ? object->etag : "(none)",
               code, full_len, (int)last_modified,
               etag ? etag : "(none)");
        privatiseObject(old_object, 0);
        new_object = makeObject(object->type, object->key, 
                                object->key_size, 1, 0, 
                                object->request, NULL);
        if(new_object == NULL) {
            do_log(L_ERROR, "Couldn't allocate object\n");
            httpServerAbort(connection, 1, 500,
                            internAtom("Couldn't allocate object"));
            return 1;
        }
        if(urlIsLocal(new_object->key, new_object->key_size))
            new_object->flags |= OBJECT_LOCAL;
    } else {
        new_object = object;
    }

    suspectDynamic =
        (!etag && last_modified < 0) ||
        (cache_control.flags &
         (CACHE_NO_HIDDEN | CACHE_NO | CACHE_NO_STORE |
          (cacheIsShared ? CACHE_PRIVATE : 0))) ||
        (cache_control.max_age >= 0 && cache_control.max_age <= 2) ||
        (cacheIsShared && 
         cache_control.s_maxage >= 0 && cache_control.s_maxage <= 5) ||
        (old_object->last_modified >= 0 && old_object->expires >= 0 && 
         (old_object->expires - old_object->last_modified <= 1)) ||
        (supersede && (old_object->date - date <= 5));

    if(suspectDynamic)
        new_object->flags |= OBJECT_DYNAMIC;
    else if(!supersede)
        new_object->flags &= ~OBJECT_DYNAMIC;
    else if(old_object->flags & OBJECT_DYNAMIC)
        new_object->flags |= OBJECT_DYNAMIC;

    new_object->age = age;
    new_object->cache_control |= cache_control.flags;
    new_object->max_age = cache_control.max_age;
    new_object->s_maxage = cache_control.s_maxage;
    new_object->flags &= ~OBJECT_FAILED;

    if(date >= 0)
        new_object->date = date;
    if(last_modified >= 0)
        new_object->last_modified = last_modified;
    if(expires >= 0)
        new_object->expires = expires;
    if(new_object->etag == NULL)
        new_object->etag = etag;
    else
        free(etag);

    switch(code) {
    case 200:
    case 300: case 301: case 302: case 303: case 307:
    case 403: case 404: case 405: case 401:
        if(new_object->message) releaseAtom(new_object->message);
        new_object->code = code;
        new_object->message = message;
        break;
    case 206: case 304: case 412:
        if(new_object->code != 200 || !new_object->message) {
            if(new_object->message) releaseAtom(new_object->message);
            new_object->code = 200;
            new_object->message = internAtom("OK");
        }
        releaseAtom(message);
        break;
    default:
        if(new_object->message) releaseAtom(new_object->message);
        new_object->code = code;
        new_object->message = retainAtom(message);
        break;
    }

    httpTweakCachability(new_object);

    if(!via)
        new_via = internAtomF("%s %s",
                              version == HTTP_11 ? "1.1" : "1.0",
                              proxyName->string);
    else
        new_via = internAtomF("%s, %s %s", via->string,
                              version == HTTP_11 ? "1.1" : "1.0",
                              proxyName->string);
    if(new_via == NULL) {
        do_log(L_ERROR, "Couldn't allocate Via.\n");
    } else {
        if(new_object->via) releaseAtom(new_object->via);
        new_object->via = new_via;
    }

    if(new_object->flags & OBJECT_INITIAL) {
        objectPartial(new_object, full_len, headers);
    } else {
        if(new_object->length < 0)
            new_object->length = full_len;
        /* XXX -- RFC 2616 13.5.3 */
        releaseAtom(headers);
    }

    if(supersede) {
        assert(new_object != old_object);
        supersedeObject(old_object);
    }

    if(new_object != old_object) {
        if(new_object->flags & OBJECT_INPROGRESS) {
            /* Make sure we don't fetch this object two times at the
               same time.  Just drop the connection. */
            releaseObject(new_object);
            httpServerFinish(connection, 1, 0);
            return 1;
        }
        old_object->flags &= ~OBJECT_VALIDATING;
        new_object->flags |= OBJECT_INPROGRESS;
        /* Signal the client side to switch to the new object -- see
           httpClientGetHandler.  If it doesn't, we'll give up on this
           request below. */
        new_object->flags |= OBJECT_MUTATING;
        request->can_mutate = new_object;
        notifyObject(old_object);
        request->can_mutate = NULL;
        new_object->flags &= ~OBJECT_MUTATING;
        old_object->flags &= ~OBJECT_INPROGRESS;
        if(request->object == old_object) {
            if(request->request)
                request->request->request = NULL;
            request->request = NULL;
            request->object = new_object;
        } else {
            assert(request->object == new_object);
        }
        releaseNotifyObject(old_object);
        old_object = NULL;
        object = new_object;
    } else {
        objectMetadataChanged(new_object, 0);
    }

    if(object->flags & OBJECT_VALIDATING) {
        object->flags &= ~OBJECT_VALIDATING;
        notifyObject(object);
    }

    if(!expect_body) {
        httpServerFinish(connection, 0, rc);
        return 1;
    }

    if(request->request == NULL) {
        httpServerFinish(connection, 1, 0);
        return 1;
    }

    if(code == 412) {
        /* 412 replies contain a useless body.  For now, we
           drop the connection. */
        httpServerFinish(connection, 1, 0);
        return 1;
    }


    if(request->flags & REQUEST_PERSISTENT) {
        if(request->method != METHOD_HEAD && 
           connection->te == TE_IDENTITY && len < 0) {
            do_log(L_ERROR, "Persistent reply with no Content-Length\n");
            /* That's potentially dangerous, as we could start reading
               arbitrary data into the object.  Unfortunately, some
               servers do that. */
            request->flags &= ~REQUEST_PERSISTENT;
        }
    }

    /* we're getting a body */
    if(content_range.from > 0)
        connection->offset = content_range.from;
    else
        connection->offset = 0;

    if(content_range.to >= 0)
        request->to = content_range.to;

    do_log(D_SERVER_OFFSET, "0x%lx(0x%lx): offset = %d\n",
           (unsigned long)connection, (unsigned long)object,
           connection->offset);

    if(connection->len > rc) {
        rc = connectionAddData(connection, rc);
        if(rc) {
            if(rc < 0) {
                if(rc == -2) {
                    do_log(L_ERROR, "Couldn't parse chunk size.\n");
                    httpServerAbort(connection, 1, 502,
                                    internAtom("Couldn't parse chunk size"));
                } else {
                    do_log(L_ERROR, "Couldn't add data to connection.\n");
                    httpServerAbort(connection, 1, 500, 
                                    internAtom("Couldn't add data "
                                               "to connection"));
                }
                return 1;
            } else {
                if(code != 206) {
                    if(object->length < 0) {
                        object->length = object->size;
                        objectMetadataChanged(object, 0);
                    } else if(object->length != object->size) {
                        httpServerAbort(connection, 1, 500, 
                                        internAtom("Inconsistent "
                                                   "object size"));
                        object->length = -1;
                        return 1;
                    }
                }
                httpServerFinish(connection, 0, 0);
                return 1;
            }
        }
    } else {
        connection->len = 0;
    }

    if(eof) {
        if(connection->te == TE_CHUNKED ||
           (object->length >= 0 && 
            connection->offset < object->length)) {
            do_log(L_ERROR, "Server closed connection.\n");
            httpServerAbort(connection, 1, 502,
                            internAtom("Server closed connection"));
            return 1;
        } else {
            if(code != 206 && eof > 0 && object->length < 0) {
                object->length = object->size;
                objectMetadataChanged(object, 0);
            }
            httpServerFinish(connection, 1, 0);
            return 1;
        }
    } else {
        return httpServerReadData(connection, 1);
    }
    return 0;

 fail:
    releaseAtom(url);
    releaseAtom(message);
    if(headers)
        releaseAtom(headers);
    if(etag)
        free(etag);
    if(via)
        releaseAtom(via);
    return 1;
}