int 
httpSpecialRequest(ObjectPtr object, int method, int from, int to,
                   HTTPRequestPtr requestor, void *closure)
{
    char buffer[1024];
    int hlen;

    if(method >= METHOD_POST) {
        return httpSpecialSideRequest(object, method, from, to,
                                      requestor, closure);
    }

    if(!(object->flags & OBJECT_INITIAL)) {
        privatiseObject(object, 0);
        supersedeObject(object);
        object->flags &= ~(OBJECT_VALIDATING | OBJECT_INPROGRESS);
        notifyObject(object);
        return 1;
    }

    hlen = snnprintf(buffer, 0, 1024,
                     "\r\nServer: polipo"
                     "\r\nContent-Type: text/html");
    object->date = current_time.tv_sec;
    object->age = current_time.tv_sec;
    object->headers = internAtomN(buffer, hlen);
    object->code = 200;
    object->message = internAtom("Okay");
    object->flags &= ~OBJECT_INITIAL;
    object->flags |= OBJECT_DYNAMIC;

    if(object->key_size == 8 && memcmp(object->key, "/polipo/", 8) == 0) {
        objectPrintf(object, 0,
                     "<!DOCTYPE HTML PUBLIC "
                     "\"-//W3C//DTD HTML 4.01 Transitional//EN\" "
                     "\"http://www.w3.org/TR/html4/loose.dtd\">\n"
                     "<html><head>\n"
                     "<title>Polipo</title>\n"
                     "</head><body>\n"
                     "<h1>Polipo</h1>\n"
                     "<p><a href=\"status?\">Status report</a>.</p>\n"
                     "<p><a href=\"config?\">Current configuration</a>.</p>\n"
                     "<p><a href=\"servers?\">Known servers</a>.</p>\n"
#ifndef NO_DISK_CACHE
                     "<p><a href=\"index?\">Disk cache index</a>.</p>\n"
#endif
                     "</body></html>\n");
        object->length = object->size;
    } else if(matchUrl("/polipo/status", object)) {
        objectPrintf(object, 0,
                     "<!DOCTYPE HTML PUBLIC "
                     "\"-//W3C//DTD HTML 4.01 Transitional//EN\" "
                     "\"http://www.w3.org/TR/html4/loose.dtd\">\n"
                     "<html><head>\n"
                     "<title>Polipo status report</title>\n"
                     "</head><body>\n"
                     "<h1>Polipo proxy on %s:%d: status report</h1>\n"
                     "<p>The %s proxy on %s:%d is %s.</p>\n"
                     "<p>There are %d public and %d private objects "
                     "currently in memory using %d KB in %d chunks "
                     "(%d KB allocated).</p>\n"
                     "<p>There are %d atoms.</p>"
                     "<p><form method=POST action=\"/polipo/status?\">"
                     "<input type=submit name=\"init-forbidden\" "
                     "value=\"Read forbidden file\"></form>\n"
                     "<form method=POST action=\"/polipo/status?\">"
                     "<input type=submit name=\"writeout-objects\" "
                     "value=\"Write out in-memory cache\"></form>\n"
                     "<form method=POST action=\"/polipo/status?\">"
                     "<input type=submit name=\"discard-objects\" "
                     "value=\"Discard in-memory cache\"></form>\n"
                     "<form method=POST action=\"/polipo/status?\">"
                     "<input type=submit name=\"reopen-log\" "
                     "value=\"Reopen log file\"></form>\n"
                     "<form method=POST action=\"/polipo/status?\">"
                     "<input type=submit name=\"free-chunk-arenas\" "
                     "value=\"Free chunk arenas\"></form></p>\n"
                     "<p><a href=\"/polipo/\">back</a></p>"
                     "</body></html>\n",
                     proxyName->string, proxyPort,
                     cacheIsShared ? "shared" : "private",
                     proxyName->string, proxyPort,
                     proxyOffline ? "off line" :
                     (relaxTransparency ? 
                      "on line (transparency relaxed)" :
                      "on line"),
                     publicObjectCount, privateObjectCount,
                     used_chunks * CHUNK_SIZE / 1024, used_chunks,
                     totalChunkArenaSize() / 1024,
                     used_atoms);
        object->expires = current_time.tv_sec;
        object->length = object->size;
    } else if(matchUrl("/polipo/config", object)) {
        fillSpecialObject(object, printConfig, NULL);
        object->expires = current_time.tv_sec + 5;
#ifndef NO_DISK_CACHE
    } else if(matchUrl("/polipo/index", object)) {
        int len;
        char *root;
        if(disableIndexing) {
            abortObject(object, 403, internAtom("Action not allowed"));
            notifyObject(object);
            return 1;
        }
        len = MAX(0, object->key_size - 14);
        root = strdup_n((char*)object->key + 14, len);
        if(root == NULL) {
            abortObject(object, 503, internAtom("Couldn't allocate root"));
            notifyObject(object);
            return 1;
        }
        writeoutObjects(1);
        fillSpecialObject(object, plainIndexDiskObjects, root);
        free(root);
        object->expires = current_time.tv_sec + 5;
    } else if(matchUrl("/polipo/recursive-index", object)) {
        int len;
        char *root;
        if(disableIndexing) {
            abortObject(object, 403, internAtom("Action not allowed"));
            notifyObject(object);
            return 1;
        }
        len = MAX(0, object->key_size - 24);
        root = strdup_n((char*)object->key + 24, len);
        if(root == NULL) {
            abortObject(object, 503, internAtom("Couldn't allocate root"));
            notifyObject(object);
            return 1;
        }
        writeoutObjects(1);
        fillSpecialObject(object, recursiveIndexDiskObjects, root);
        free(root);
        object->expires = current_time.tv_sec + 20;
#endif
    } else if(matchUrl("/polipo/servers", object)) {
        if(disableServersList) {
            abortObject(object, 403, internAtom("Action not allowed"));
            notifyObject(object);
            return 1;
        }
        fillSpecialObject(object, serversList, NULL);
        object->expires = current_time.tv_sec + 2;
    } else {
        abortObject(object, 404, internAtom("Not found"));
    }

    object->flags &= ~OBJECT_VALIDATING;
    notifyObject(object);
    return 1;
}