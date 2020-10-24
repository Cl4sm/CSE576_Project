void
httpServerAbortRequest(HTTPRequestPtr request, int fail,
                       int code, AtomPtr message)
{
    if(request->connection && request == request->connection->request) {
        httpServerAbort(request->connection, fail, code, message);
    } else {
        HTTPRequestPtr requestor = request->request;
        if(requestor) {
            requestor->request = NULL;
            request->request = NULL;
            httpClientError(requestor, code, retainAtom(message));
        }
        if(fail) {
            request->object->flags |= OBJECT_FAILED;
            if(request->object->flags & OBJECT_INITIAL)
                abortObject(request->object, code, retainAtom(message));
            notifyObject(request->object);
        }
        releaseAtom(message);
    }
}