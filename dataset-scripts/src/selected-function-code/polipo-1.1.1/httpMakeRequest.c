HTTPRequestPtr 
httpMakeRequest()
{
    HTTPRequestPtr request;
    request = malloc(sizeof(HTTPRequestRec));
    if(request == NULL)
        return NULL;
    request->flags = 0;
    request->connection = NULL;
    request->object = NULL;
    request->method = METHOD_UNKNOWN;
    request->from = 0;
    request->to = -1;
    request->cache_control = no_cache_control;
    request->condition = NULL;
    request->via = NULL;
    request->chandler = NULL;
    request->can_mutate = NULL;
    request->error_code = 0;
    request->error_message = NULL;
    request->error_headers = NULL;
    request->headers = NULL;
    request->time0 = null_time;
    request->time1 = null_time;
    request->request = NULL;
    request->next = NULL;
    return request;
}