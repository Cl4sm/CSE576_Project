static int
httpServeObjectFinishHandler(int status,
                             FdEventHandlerPtr event,
                             StreamRequestPtr srequest)
{
    HTTPConnectionPtr connection = srequest->data;
    HTTPRequestPtr request = connection->request;

    (void)request;
    assert(!request->chandler);

    if(status == 0 && !streamRequestDone(srequest))
        return 0;

    httpSetTimeout(connection, -1);

    if(status < 0) {
        do_log(L_ERROR, "Couldn't terminate chunked reply\n");
        httpClientFinish(connection, 1);
    } else {
        httpClientFinish(connection, 0);
    }
    return 1;
}