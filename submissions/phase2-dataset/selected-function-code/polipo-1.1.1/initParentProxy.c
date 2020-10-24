static int
initParentProxy()
{
    AtomPtr host, port_atom;
    int rc, port;

    if(parentHost) {
        releaseAtom(parentHost);
        parentHost = NULL;
    }
    if(parentPort >= 0)
        parentPort = -1;

    if(parentProxy != NULL && parentProxy->length == 0) {
        releaseAtom(parentProxy);
        parentProxy = NULL;
    }

    if(parentProxy == NULL)
        return 1;

    rc = atomSplit(parentProxy, ':', &host, &port_atom);
    if(rc <= 0) {
        do_log(L_ERROR, "Couldn't parse parentProxy.");
        releaseAtom(parentProxy);
        parentProxy = NULL;
        return -1;
    }

    port = atoi(port_atom->string);
    if(port <= 0 || port >= 0x10000) {
        releaseAtom(host);
        releaseAtom(port_atom);
        do_log(L_ERROR, "Couldn't parse parentProxy.");
        releaseAtom(parentProxy);
        parentProxy = NULL;
        return -1;
    }

    parentHost = host;
    parentPort = port;
    return 1;
}