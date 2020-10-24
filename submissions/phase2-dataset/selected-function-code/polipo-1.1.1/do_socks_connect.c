int
do_socks_connect(char *name, int port,
                 int (*handler)(int, SocksRequestPtr),
                 void *data)
{
    SocksRequestPtr request = malloc(sizeof(SocksRequestRec));
    SocksRequestRec request_nomem;
    if(request == NULL)
        goto nomem;

    request->name = internAtomLowerN(name, strlen(name));
    if(request->name == NULL) {
        free(request);
        goto nomem;
    }

    request->port = port;
    request->fd = -1;
    request->handler = handler;
    request->buf = NULL;
    request->data = data;

    if(socksProxyAddress == NULL) {
        do_gethostbyname(socksProxyHost->string, 0,
                         socksDnsHandler,
                         request);
        return 1;
    }

    return do_socks_connect_common(request);

 nomem:
    request_nomem.name = internAtomLowerN(name, strlen(name));
    request_nomem.port = port;
    request_nomem.handler = handler;
    request_nomem.buf = NULL;
    request_nomem.data = data;

    handler(-ENOMEM, &request_nomem);
    releaseAtom(request_nomem.name);
    return 1;
}