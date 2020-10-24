int
urlForbidden(AtomPtr url,
             int (*handler)(int, AtomPtr, AtomPtr, AtomPtr, void*),
             void *closure)
{
    int forbidden = urlIsMatched(url->string, url->length,
                                 forbiddenDomains, forbiddenRegex);
    int code = 0;
    AtomPtr message = NULL, headers = NULL;


    if(forbidden) {
        message = internAtomF("Forbidden URL %s", url->string);
        if(forbiddenUrl) {
            code = forbiddenRedirectCode;
            headers = internAtomF("\r\nLocation: %s", forbiddenUrl->string);
        } else {
            code = 403;
        }
    }

#ifndef NO_REDIRECTOR
    if(code == 0 && redirector) {
        RedirectRequestPtr request;
        request = malloc(sizeof(RedirectRequestRec));
        if(request == NULL) {
            do_log(L_ERROR, "Couldn't allocate redirect request.\n");
            goto done;
        }
        request->url = url;
        request->handler = handler;
        request->data = closure;
        if(redirector_request_first == NULL)
            redirector_request_first = request;
        else
            redirector_request_last->next = request;
        redirector_request_last = request;
        request->next = NULL;
        if(request == redirector_request_first)
            redirectorTrigger();
        return 1;
    }

#endif

 done:
    handler(code, url, message, headers, closure);
    return 1;
}