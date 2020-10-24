static int
pipelineIsSmall(HTTPConnectionPtr connection)
{
    HTTPRequestPtr request = connection->request;

    if(pipelineAdditionalRequests <= 0)
        return 0;
    else if(pipelineAdditionalRequests >= 2)
        return 1;

    if(!request)
        return 1;
    if(request->next || !(request->flags & REQUEST_PERSISTENT))
        return 0;
    if(request->method == METHOD_HEAD || 
       request->method == METHOD_CONDITIONAL_GET)
        return 1;
    if(request->to >= 0 && connection->server->rate > 0 &&
       request->to - request->from < connection->server->rate * 
       smallRequestTime)
        return 1;
    return 0;
}