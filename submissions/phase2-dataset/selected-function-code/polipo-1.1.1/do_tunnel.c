void 
do_tunnel(int fd, char *buf, int offset, int len, AtomPtr url)
{
    TunnelPtr tunnel;
    int port;
    char *p, *q;

    tunnel = makeTunnel(fd, buf, offset, len);
    if(tunnel == NULL) {
        do_log(L_ERROR, "Couldn't allocate tunnel.\n");
        releaseAtom(url);
        dispose_chunk(buf);
        CLOSE(fd);
        return;
    }

    if(proxyOffline) {
        do_log(L_INFO, "Attemted CONNECT when disconnected.\n");
        releaseAtom(url);
        tunnelError(tunnel, 502,
                    internAtom("Cannot CONNECT when disconnected."));
        return;
    }

    p = memrchr(url->string, ':', url->length);
    q = NULL;
    if(p)
        port = strtol(p + 1, &q, 10);
    if(!p || q != url->string + url->length) {
        do_log(L_ERROR, "Couldn't parse CONNECT.\n");
        releaseAtom(url);
        tunnelError(tunnel, 400, internAtom("Couldn't parse CONNECT"));
        return;
    }
    tunnel->hostname = internAtomLowerN(url->string, p - url->string);
    if(tunnel->hostname == NULL) {
        releaseAtom(url);
        tunnelError(tunnel, 501, internAtom("Couldn't allocate hostname"));
        return;
    }

    if(!intListMember(port, tunnelAllowedPorts)) {
        releaseAtom(url);
        tunnelError(tunnel, 403, internAtom("Forbidden port"));
        return;
    }
    tunnel->port = port;
    
    if (tunnelIsMatched(url->string, url->length, 
			tunnel->hostname->string, tunnel->hostname->length)) {
        releaseAtom(url);
        tunnelError(tunnel, 404, internAtom("Forbidden tunnel"));
	logTunnel(tunnel,1);
        return;
    }
    
    logTunnel(tunnel,0);
    
    releaseAtom(url);

    if(socksParentProxy)
        do_socks_connect(parentHost ?
                         parentHost->string : tunnel->hostname->string,
                         parentHost ? parentPort : tunnel->port,
                         tunnelSocksHandler, tunnel);
    else
        do_gethostbyname(parentHost ?
                         parentHost->string : tunnel->hostname->string, 0,
                         tunnelDnsHandler, tunnel);
}