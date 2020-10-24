int
httpClientSideRequest(HTTPRequestPtr request)
{
    HTTPConnectionPtr connection = request->connection;

    if(request->from < 0 || request->to >= 0) {
        httpClientNoticeError(request, 501,
                              internAtom("Partial requests not implemented"));
        httpClientDiscardBody(connection);
        return 1;
    }
    if(connection->reqte != TE_IDENTITY) {
        httpClientNoticeError(request, 501,
                              internAtom("Chunked requests not implemented"));
        httpClientDiscardBody(connection);
        return 1;
    }
    if(connection->bodylen < 0) {
        httpClientNoticeError(request, 502,
                              internAtom("POST or PUT without "
                                         "Content-Length"));
        httpClientDiscardBody(connection);
        return 1;
    }
    if(connection->reqlen < 0) {
        httpClientNoticeError(request, 502,
                              internAtom("Incomplete POST or PUT"));
        httpClientDiscardBody(connection);
        return 1;
    }
        
    return httpClientNoticeRequest(request, 0);
}