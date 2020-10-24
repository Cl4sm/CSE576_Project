int
httpServerIndirectHandlerCommon(HTTPConnectionPtr connection, int eof)
{
    HTTPRequestPtr request = connection->request;

    assert(eof >= 0);
    assert(request->object->flags & OBJECT_INPROGRESS);

    if(connection->len > 0) {
        int rc;
        rc = connectionAddData(connection, 0);
        if(rc) {
            if(rc < 0) {
                if(rc == -2) {
                    do_log(L_ERROR, "Couldn't parse chunk size.\n");
                    httpServerAbort(connection, 1, 502,
                                    internAtom("Couldn't parse chunk size"));
                } else {
                    do_log(L_ERROR, "Couldn't add data to connection.\n");
                    httpServerAbort(connection, 1, 500,
                                    internAtom("Couldn't add data "
                                               "to connection"));
                }
                return 1;
            } else {
                if(request->to < 0) {
                    if(request->object->length < 0) {
                        request->object->length = request->object->size;
                        objectMetadataChanged(request->object, 0);
                    } else if(request->object->length != 
                              request->object->size) {
                        request->object->length = -1;
                        httpServerAbort(connection, 1, 502,
                                        internAtom("Inconsistent "
                                                   "object size"));
                        return 1;
                    }
                }
                httpServerFinish(connection, 0, 0);
            }
            return 1;
        }
    }

    if(eof && connection->len == 0) {
        if(connection->te == TE_CHUNKED ||
           (request->to >= 0 && connection->offset < request->to)) {
            do_log(L_ERROR, "Server dropped connection.\n");
            httpServerAbort(connection, 1, 502, 
                            internAtom("Server dropped connection"));
            return 1;
        } else {
            if(request->object->length < 0 && eof > 0 &&
               (request->to < 0 || request->to > request->object->size)) {
                request->object->length = request->object->size;
                objectMetadataChanged(request->object, 0);
            }
            httpServerFinish(connection, 1, 0);
            return 1;
        }
    } else {
        return httpServerReadData(connection, 0);
    }
}