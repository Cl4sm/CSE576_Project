int
redirectorStreamHandler2(int status,
                         FdEventHandlerPtr event,
                         StreamRequestPtr srequest)
{
    RedirectRequestPtr request = (RedirectRequestPtr)srequest->data;
    char *c;
    AtomPtr message;
    AtomPtr headers;
    int code;

    if(status < 0) {
        do_log_error(L_ERROR, -status, "Read from redirector failed");
        request->handler(status, request->url, NULL, NULL, request->data);
        goto kill;
    }
    c = memchr(redirector_buffer, '\n', srequest->offset);
    if(!c) {
        if(!status && srequest->offset < REDIRECTOR_BUFFER_SIZE)
            return 0;
        do_log(L_ERROR, "Redirector returned incomplete reply.\n");
        request->handler(-EREDIRECTOR, request->url, NULL, NULL, request->data);
        goto kill;
    }
    *c = '\0';

    if(srequest->offset > c + 1 - redirector_buffer)
        do_log(L_WARN, "Stray bytes in redirector output.\n");

    if(c > redirector_buffer + 1 &&
       (c - redirector_buffer != request->url->length ||
        memcmp(redirector_buffer, request->url->string,
               request->url->length) != 0)) {
        code = redirectorRedirectCode;
        message = internAtom("Redirected by external redirector");
        if(message == NULL) {
            request->handler(-ENOMEM, request->url, NULL, NULL, request->data);
            goto kill;
        }

        headers = internAtomF("\r\nLocation: %s", redirector_buffer);
        if(headers == NULL) {
            releaseAtom(message);
            request->handler(-ENOMEM, request->url, NULL, NULL, request->data);
            goto kill;
        }
    } else {
        code = 0;
        message = NULL;
        headers = NULL;
    }
    request->handler(code, request->url,
                     message, headers, request->data);
    goto cont;

 cont:
    redirectorDestroyRequest(request);
    redirectorTrigger();
    return 1;

 kill:
    redirectorKill();
    goto cont;
}