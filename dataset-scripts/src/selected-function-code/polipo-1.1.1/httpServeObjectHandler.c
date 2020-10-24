int
httpServeObjectHandler(int status, ConditionHandlerPtr chandler)
{
    HTTPConnectionPtr connection = *(HTTPConnectionPtr*)chandler->data;
    HTTPRequestPtr request = connection->request;
    int rc;

    if((request->object->flags & OBJECT_ABORTED) || status < 0) {
        shutdown(connection->fd, 1);
        httpSetTimeout(connection, 10);
        /* httpServeChunk will take care of the error. */
    }

    httpSetTimeout(connection, -1);

    request->chandler = NULL;
    rc = delayedHttpServeChunk(connection);
    if(rc < 0) {
        do_log(L_ERROR, "Couldn't schedule serving.\n");
        abortObject(request->object, 503, 
                    internAtom("Couldn't schedule serving"));
    }
    return 1;
}