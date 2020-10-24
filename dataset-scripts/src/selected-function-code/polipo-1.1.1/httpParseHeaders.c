int
httpParseHeaders(int client, AtomPtr url,
                 const char *buf, int start, HTTPRequestPtr request,
                 AtomPtr *headers_return,
                 int *len_return, CacheControlPtr cache_control_return,
                 HTTPConditionPtr *condition_return, int *te_return,
                 time_t *date_return, time_t *last_modified_return,
                 time_t *expires_return, time_t *polipo_age_return,
                 time_t *polipo_access_return, int *polipo_body_offset_return,
                 int *age_return, char **etag_return, AtomPtr *expect_return,
                 HTTPRangePtr range_return, HTTPRangePtr content_range_return,
                 char **location_return, AtomPtr *via_return,
                 AtomPtr *auth_return)
{
    int local = url ? urlIsLocal(url->string, url->length) : 0;
    char hbuf_small[512];
    char *hbuf = hbuf_small;
    int hbuf_size = 512, hbuf_length = 0;
    int i, j,
        name_start, name_end, value_start, value_end, 
        token_start, token_end, end;
    AtomPtr name = NULL;
    time_t date = -1, last_modified = -1, expires = -1, polipo_age = -1,
        polipo_access = -1, polipo_body_offset = -1;
    int len = -1;
    CacheControlRec cache_control;
    char *endptr;
    int te = TE_IDENTITY;
    int age = -1;
    char *etag = NULL, *ifrange = NULL;
    int persistent = (!request || (request->connection->version != HTTP_10));
    char *location = NULL;
    AtomPtr via = NULL;
    AtomPtr auth = NULL;
    AtomPtr expect = NULL;
    HTTPConditionPtr condition;
    time_t ims = -1, inms = -1;
    char *im = NULL, *inm = NULL;
    AtomListPtr hopToHop = NULL;
    HTTPRangeRec range = {-1, -1, -1}, content_range = {-1, -1, -1};
    int haveCacheControl = 0;
 
#define RESIZE_HBUF() \
    do { \
        hbuf = resize_hbuf(hbuf, &hbuf_size, hbuf_small); \
        if(hbuf == NULL) \
            goto fail; \
    } while(0)

    cache_control.flags = 0;
    cache_control.max_age = -1;
    cache_control.s_maxage = -1;
    cache_control.min_fresh = -1;
    cache_control.max_stale = -1;
    
    i = start;

    while(1) {
        i = parseHeaderLine(buf, i,
                            &name_start, &name_end, &value_start, &value_end);
        if(i < 0) {
            do_log(L_ERROR, "Couldn't find end of header line.\n");
            goto fail;
        }

        if(name_start == -1)
            break;

        if(name_start < 0)
            continue;

        name = internAtomLowerN(buf + name_start, name_end - name_start);

        if(name == atomConnection) {
            j = getNextTokenInList(buf, value_start, 
                                   &token_start, &token_end, NULL, NULL,
                                   &end);
            while(1) {
                if(j < 0) {
                    do_log(L_ERROR, "Couldn't parse Connection: ");
                    do_log_n(L_ERROR, buf + value_start, 
                             value_end - value_start);
                    do_log(L_ERROR, ".\n");
                    goto fail;
                }
                if(token_compare(buf, token_start, token_end, "close")) {
                    persistent = 0;
                } else if(token_compare(buf, token_start, token_end, 
                                        "keep-alive")) {
                    persistent = 1;
                } else {
                    if(hopToHop == NULL)
                        hopToHop = makeAtomList(NULL, 0);
                    if(hopToHop == NULL) {
                        do_log(L_ERROR, "Couldn't allocate atom list.\n");
                        goto fail;
                    }
                    atomListCons(internAtomLowerN(buf + token_start,
                                                  token_end - token_start),
                                 hopToHop);
                }
                if(end)
                    break;
                j = getNextTokenInList(buf, j, 
                                       &token_start, &token_end, NULL, NULL,
                                       &end);
            }
        } else if(name == atomCacheControl)
            haveCacheControl = 1;

        releaseAtom(name);
        name = NULL;
    }
    
    i = start;

    while(1) {
        i = parseHeaderLine(buf, i, 
                            &name_start, &name_end, &value_start, &value_end);
        if(i < 0) {
            do_log(L_ERROR, "Couldn't find end of header line.\n");
            goto fail;
        }

        if(name_start == -1)
            break;

        if(name_start < 0) {
            do_log(L_WARN, "Couldn't parse header line.\n");
            if(laxHttpParser)
                continue;
            else
                goto fail;
        }

        name = internAtomLowerN(buf + name_start, name_end - name_start);
        
        if(name == atomProxyConnection) {
            j = getNextTokenInList(buf, value_start, 
                                   &token_start, &token_end, NULL, NULL,
                                   &end);
            while(1) {
                if(j < 0) {
                    do_log(L_WARN, "Couldn't parse Proxy-Connection:");
                    do_log_n(L_WARN, buf + value_start, 
                             value_end - value_start);
                    do_log(L_WARN, ".\n");
                    persistent = 0;
                    break;
                }
                if(token_compare(buf, token_start, token_end, "close")) {
                    persistent = 0;
                } else if(token_compare(buf, token_start, token_end, 
                                        "keep-alive")) {
                    persistent = 1;
                }
                if(end)
                    break;
                j = getNextTokenInList(buf, j, 
                                       &token_start, &token_end, NULL, NULL,
                                       &end);
            }
        } else if(name == atomContentLength) {
            j = skipWhitespace(buf, value_start);
            if(j < 0) {
                do_log(L_WARN, "Couldn't parse Content-Length: \n");
                do_log_n(L_WARN, buf + value_start, value_end - value_start);
                do_log(L_WARN, ".\n");
                len = -1;
            } else {
                errno = 0;
                len = strtol(buf + value_start, &endptr, 10);
                if(errno == ERANGE || endptr <= buf + value_start) {
                    do_log(L_WARN, "Couldn't parse Content-Length: \n");
                    do_log_n(L_WARN, buf + value_start, 
                             value_end - value_start);
                    do_log(L_WARN, ".\n");
                    len = -1;
                }
            }
        } else if((!local && name == atomProxyAuthorization) ||
                  (local && name == atomAuthorization)) {
            if(auth_return) {
                auth = internAtomN(buf + value_start, value_end - value_start);
                if(auth == NULL) {
                    do_log(L_ERROR, "Couldn't allocate authorization.\n");
                    goto fail;
                }
            }
        } else if(name == atomReferer) {
            int h;
            if(censorReferer == 0 || 
               (censorReferer == 1 && url != NULL &&
                urlSameHost(url->string, url->length,
                            buf + value_start, value_end - value_start))) {
                while(hbuf_length > hbuf_size - 2)
                    RESIZE_HBUF();
                hbuf[hbuf_length++] = '\r';
                hbuf[hbuf_length++] = '\n';
                do {
                    h = snnprint_n(hbuf, hbuf_length, hbuf_size,
                                   buf + name_start, value_end - name_start);
                    if(h < 0) RESIZE_HBUF();
                } while(h < 0);
                hbuf_length = h;
            }
        } else if(name == atomTrailer || name == atomUpgrade) {
            do_log(L_ERROR, "Trailers or upgrade present.\n");
            goto fail;
        } else if(name == atomDate || name == atomExpires ||
                  name == atomIfModifiedSince || 
                  name == atomIfUnmodifiedSince ||
                  name == atomLastModified ||
                  name == atomXPolipoDate || name == atomXPolipoAccess) {
            time_t t;
            j = parse_time(buf, value_start, value_end, &t);
            if(j < 0) {
                if(name != atomExpires) {
                    do_log(L_WARN, "Couldn't parse %s: ", name->string);
                    do_log_n(L_WARN, buf + value_start,
                             value_end - value_start);
                    do_log(L_WARN, "\n");
                }
                t = -1;
            }
            if(name == atomDate) {
                if(t >= 0)
                    date = t;
            } else if(name == atomExpires) {
                if(t >= 0)
                    expires = t;
                else
                    expires = 0;
            } else if(name == atomLastModified)
                last_modified = t;
            else if(name == atomIfModifiedSince)
                ims = t;
            else if(name == atomIfUnmodifiedSince)
                inms = t;
            else if(name == atomXPolipoDate)
                polipo_age = t;
            else if(name == atomXPolipoAccess)
                polipo_access = t;
        } else if(name == atomAge) {
            j = skipWhitespace(buf, value_start);
            if(j < 0) {
                age = -1;
            } else {
                errno = 0;
                age = strtol(buf + value_start, &endptr, 10);
                if(errno == ERANGE || endptr <= buf + value_start)
                    age = -1;
            }
            if(age < 0) {
                do_log(L_WARN, "Couldn't parse age: \n");
                do_log_n(L_WARN, buf + value_start, value_end - value_start);
                do_log(L_WARN, " -- ignored.\n");
            }
        } else if(name == atomXPolipoBodyOffset) {
            j = skipWhitespace(buf, value_start);
            if(j < 0) {
                do_log(L_ERROR, "Couldn't parse body offset.\n");
                goto fail;
            } else {
                errno = 0;
                polipo_body_offset = strtol(buf + value_start, &endptr, 10);
                if(errno == ERANGE || endptr <= buf + value_start) {
                    do_log(L_ERROR, "Couldn't parse body offset.\n");
                    goto fail;
                }
            }
        } else if(name == atomTransferEncoding) {
            if(token_compare(buf, value_start, value_end, "identity"))
                te = TE_IDENTITY;
            else if(token_compare(buf, value_start, value_end, "chunked"))
                te = TE_CHUNKED;
            else
                te = TE_UNKNOWN;
        } else if(name == atomETag ||
                  name == atomIfNoneMatch || name == atomIfMatch ||
                  name == atomIfRange) {
            int x, y;
            int weak;
            char *e;
            j = getNextETag(buf, value_start, &x, &y, &weak);
            if(j < 0) {
                if(buf[value_start] != '\r' && buf[value_start] != '\n')
                    do_log(L_ERROR, "Couldn't parse ETag.\n");
            } else if(weak) {
                do_log(L_WARN, "Server returned weak ETag -- ignored.\n");
            } else {
                e = strdup_n(buf + x, y - x);
                if(e == NULL) goto fail;
                if(name == atomETag) {
                    if(!etag)
                        etag = e;
                    else
                        free(e);
                } else if(name == atomIfNoneMatch) {
                    if(!inm)
                        inm = e;
                    else
                        free(e);
                } else if(name == atomIfMatch) {
                    if(!im)
                        im = e;
                    else
                        free(e);
                } else if(name == atomIfRange) {
                    if(!ifrange)
                        ifrange = e;
                    else
                        free(e);
                } else {
                    abort();
                }
            }
        } else if(name == atomCacheControl) {
            int v_start, v_end;
            j = getNextTokenInList(buf, value_start, 
                                   &token_start, &token_end, 
                                   &v_start, &v_end,
                                   &end);
            while(1) {
                if(j < 0) {
                    do_log(L_WARN, "Couldn't parse Cache-Control.\n");
                    cache_control.flags |= CACHE_NO;
                    break;
                }
                if(token_compare(buf, token_start, token_end, "no-cache")) {
                    cache_control.flags |= CACHE_NO;
                } else if(token_compare(buf, token_start, token_end,
                                        "public")) {
                    cache_control.flags |= CACHE_PUBLIC;
                } else if(token_compare(buf, token_start, token_end, 
                                        "private")) {
                    cache_control.flags |= CACHE_PRIVATE;
                } else if(token_compare(buf, token_start, token_end, 
                                        "no-store")) {
                    cache_control.flags |= CACHE_NO_STORE;
                } else if(token_compare(buf, token_start, token_end, 
                                        "no-transform")) {
                    cache_control.flags |= CACHE_NO_TRANSFORM;
                } else if(token_compare(buf, token_start, token_end,
                                        "must-revalidate") ||
                          token_compare(buf, token_start, token_end,
                                        "must-validate")) { /* losers */
                    cache_control.flags |= CACHE_MUST_REVALIDATE;
                } else if(token_compare(buf, token_start, token_end, 
                                        "proxy-revalidate")) {
                    cache_control.flags |= CACHE_PROXY_REVALIDATE;
                } else if(token_compare(buf, token_start, token_end,
                                        "only-if-cached")) {
                    cache_control.flags |= CACHE_ONLY_IF_CACHED;
                } else if(token_compare(buf, token_start, token_end,
                                        "max-age") ||
                          token_compare(buf, token_start, token_end,
                                        "maxage") || /* losers */
                          token_compare(buf, token_start, token_end,
                                        "s-maxage") ||
                          token_compare(buf, token_start, token_end,
                                        "min-fresh")) {
                    parseCacheControl(buf, token_start, token_end,
                                      v_start, v_end,
                                      &cache_control.max_age);
                } else if(token_compare(buf, token_start, token_end,
                                        "max-stale")) {
                    parseCacheControl(buf, token_start, token_end,
                                      v_start, v_end,
                                      &cache_control.max_stale);
                } else {
                    do_log(L_WARN, "Unsupported Cache-Control directive ");
                    do_log_n(L_WARN, buf + token_start, 
                             (v_end >= 0 ? v_end : token_end) - token_start);
                    do_log(L_WARN, " -- ignored.\n");
                }
                if(end)
                    break;
                j = getNextTokenInList(buf, j, 
                                       &token_start, &token_end,
                                       &v_start, &v_end,
                                       &end);
            }
        } else if(name == atomContentRange) {
            if(!client) {
                j = parseContentRange(buf, value_start, 
                                      &content_range.from, &content_range.to, 
                                      &content_range.full_length);
                if(j < 0) {
                    do_log(L_ERROR, "Couldn't parse Content-Range: ");
                    do_log_n(L_ERROR, buf + value_start, 
                             value_end - value_start);
                    do_log(L_ERROR, "\n");
                    goto fail;
                }
            } else {
                do_log(L_ERROR, "Content-Range from client.\n");
                goto fail;
            }
        } else if(name == atomRange) {
            if(client) {
                j = parseRange(buf, value_start, &range.from, &range.to);
                if(j < 0) {
                    do_log(L_WARN, "Couldn't parse Range -- ignored.\n");
                    range.from = -1;
                    range.to = -1;
                }
            } else {
                do_log(L_WARN, "Range from server -- ignored\n");
            }
        } else if(name == atomXPolipoLocation) {
            if(location_return) {
                location = 
                    strdup_n(buf + value_start, value_end - value_start);
                if(location == NULL) {
                    do_log(L_ERROR, "Couldn't allocate location.\n");
                    goto fail;
                }
            }
        } else if(name == atomVia) {
            if(via_return) {
                AtomPtr new_via, full_via;
                new_via =
                    internAtomN(buf + value_start, value_end - value_start);
                if(new_via == NULL) {
                    do_log(L_ERROR, "Couldn't allocate via.\n");
                    goto fail;
                }
                if(via) {
                    full_via =
                        internAtomF("%s, %s", via->string, new_via->string);
                    releaseAtom(new_via);
                    if(full_via == NULL) {
                        do_log(L_ERROR, "Couldn't allocate via");
                        goto fail;
                    }
                    releaseAtom(via);
                    via = full_via;
                } else {
                    via = new_via;
                }
            }
        } else if(name == atomExpect) {
            if(expect_return) {
                expect = internAtomLowerN(buf + value_start, 
                                          value_end - value_start);
                if(expect == NULL) {
                    do_log(L_ERROR, "Couldn't allocate expect.\n");
                    goto fail;
                }
            }
        } else {
            if(!client && name == atomContentType) {
                if(token_compare(buf, value_start, value_end,
                                 "multipart/byteranges")) {
                    do_log(L_ERROR, 
                           "Server returned multipart/byteranges -- yuck!\n");
                    goto fail;
                }
            } 
            if(name == atomVary) {
                if(!token_compare(buf, value_start, value_end, "host") &&
                   !token_compare(buf, value_start, value_end, "*")) {
                    /* What other vary headers should be ignored? */
                    do_log(L_VARY, "Vary header present (");
                    do_log_n(L_VARY,
                             buf + value_start, value_end - value_start);
                    do_log(L_VARY, ").\n");
                }
                cache_control.flags |= CACHE_VARY;
            } else if(name == atomAuthorization) {
                cache_control.flags |= CACHE_AUTHORIZATION;
            } 

            if(name == atomPragma) {
                /* Pragma is only defined for the client, and the only
                   standard value is no-cache (RFC 1945, 10.12).
                   However, we honour a Pragma: no-cache for both the client
                   and the server when there's no Cache-Control header.  In
                   all cases, we pass the Pragma header to the next hop. */
                if(!haveCacheControl) {
                    j = getNextTokenInList(buf, value_start,
                                           &token_start, &token_end, NULL, NULL,
                                           &end);
                    while(1) {
                        if(j < 0) {
                            do_log(L_WARN, "Couldn't parse Pragma.\n");
                            cache_control.flags |= CACHE_NO;
                            break;
                        }
                        if(token_compare(buf, token_start, token_end,
                                         "no-cache"))
                            cache_control.flags = CACHE_NO;
                        if(end)
                            break;
                        j = getNextTokenInList(buf, j, &token_start, &token_end,
                                               NULL, NULL, &end);
                    }
                }
            }
            if(!client &&
               (name == atomSetCookie || 
                name == atomCookie || name == atomCookie2))
                cache_control.flags |= CACHE_COOKIE;

            if(hbuf) {
                if(name != atomConnection && name != atomHost &&
                   name != atomAcceptRange && name != atomTE &&
                   name != atomProxyAuthenticate &&
                   name != atomKeepAlive &&
                   (!hopToHop || !atomListMember(name, hopToHop)) &&
                   !atomListMember(name, censoredHeaders)) {
                    int h;
                    while(hbuf_length > hbuf_size - 2)
                        RESIZE_HBUF();
                    hbuf[hbuf_length++] = '\r';
                    hbuf[hbuf_length++] = '\n';
                    do {
                        h = snnprint_n(hbuf, hbuf_length, hbuf_size,
                                       buf + name_start, 
                                       value_end - name_start);
                        if(h < 0) RESIZE_HBUF();
                    } while(h < 0);
                    hbuf_length = h;
                }
            }
        }
        releaseAtom(name);
        name = NULL;
    }

    if(headers_return) {
        AtomPtr pheaders = NULL; 
        pheaders = internAtomN(hbuf, hbuf_length);
        if(!pheaders)
            goto fail;
        *headers_return = pheaders;
    }
    if(hbuf != hbuf_small)
        free(hbuf);
    hbuf = NULL;
    hbuf_size = 0;

    if(request)
        if(!persistent)
            request->flags &= ~REQUEST_PERSISTENT;

    if(te != TE_IDENTITY) len = -1;
    if(len_return) *len_return = len;
    if(cache_control_return) *cache_control_return = cache_control;
    if(condition_return) {
        if(ims >= 0 || inms >= 0 || im || inm || ifrange) {
            condition = httpMakeCondition();
            if(condition) {
                condition->ims = ims;
                condition->inms = inms;
                condition->im = im;
                condition->inm = inm;
                condition->ifrange = ifrange;
            } else {
                do_log(L_ERROR, "Couldn't allocate condition.\n");
                if(im) free(im);
                if(inm) free(inm);
            }
        } else {
            condition = NULL;
        }
        *condition_return = condition;
    } else {
        assert(!im && !inm);
    }
            
    if(te_return) *te_return = te;
    if(date_return) *date_return = date;
    if(last_modified_return) *last_modified_return = last_modified;
    if(expires_return) *expires_return = expires;
    if(polipo_age_return) *polipo_age_return = polipo_age;
    if(polipo_access_return) *polipo_access_return = polipo_access;
    if(polipo_body_offset_return)
        *polipo_body_offset_return = polipo_body_offset;
    if(age_return) *age_return = age;
    if(etag_return)
        *etag_return = etag;
    else {
        if(etag) free(etag);
    }
    if(range_return) *range_return = range;
    if(content_range_return) *content_range_return = content_range;
    if(location_return) {
        *location_return = location;
    } else {
        if(location)
            free(location);
    }
    if(via_return)
        *via_return = via;
    else {
        if(via)
            releaseAtom(via);
    }
    if(expect_return)
        *expect_return = expect;
    else {
        if(expect)
            releaseAtom(expect);
    }
    if(auth_return)
        *auth_return = auth;
    else {
        if(auth)
            releaseAtom(auth);
    }
    if(hopToHop) destroyAtomList(hopToHop);
    return i;

 fail:
    if(hbuf && hbuf != hbuf_small) free(hbuf);
    if(name) releaseAtom(name);
    if(etag) free(etag);
    if(location) free(location);
    if(via) releaseAtom(via);
    if(expect) releaseAtom(expect);
    if(auth) releaseAtom(auth);
    if(hopToHop) destroyAtomList(hopToHop);
        
    return -1;
#undef RESIZE_HBUF
}