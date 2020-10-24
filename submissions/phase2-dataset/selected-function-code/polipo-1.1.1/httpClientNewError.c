int
httpClientNewError(HTTPConnectionPtr connection, int method, int persist,
                   int code, AtomPtr message)
{
    HTTPRequestPtr request;
    request = httpMakeRequest();
    if(request == NULL) {
        do_log(L_ERROR, "Couldn't allocate error request.\n");
        httpClientFinish(connection, 1);
        return 1;
    }
    request->method = method;
    if(persist)
        request->flags |= REQUEST_PERSISTENT;
    else
        request->flags &= ~REQUEST_PERSISTENT;
    request->error_code = code;
    request->error_message = message;

    httpQueueRequest(connection, request);
    httpClientNoticeRequest(request, 0);
    return 1;
}