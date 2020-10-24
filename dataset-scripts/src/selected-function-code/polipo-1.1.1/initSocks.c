void
initSocks()
{
    int port = -1;
    AtomPtr host = NULL, port_atom;
    int rc;

    if(socksParentProxy != NULL && socksParentProxy->length == 0) {
        releaseAtom(socksParentProxy);
        socksParentProxy = NULL;
    }

    if(socksParentProxy) {
        rc = atomSplit(socksParentProxy, ':', &host, &port_atom);
        if(rc <= 0) {
            do_log(L_ERROR, "Couldn't parse socksParentProxy");
            exit(1);
        }
        port = atoi(port_atom->string);
        releaseAtom(port_atom);
    }

    if(socksProxyHost)
        releaseAtom(socksProxyHost);
    socksProxyHost = host;
    socksProxyPort = port;
    if(socksProxyAddress)
        releaseAtom(socksProxyAddress);
    socksProxyAddress = NULL;
    socksProxyAddressIndex = -1;

    if(socksProxyType != aSocks4a && socksProxyType != aSocks5) {
        do_log(L_ERROR, "Unknown socksProxyType %s\n", socksProxyType->string);
        exit(1);
    }
}