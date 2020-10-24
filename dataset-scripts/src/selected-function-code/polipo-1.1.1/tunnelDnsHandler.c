static int
tunnelDnsHandler(int status, GethostbynameRequestPtr request)
{
    TunnelPtr tunnel = request->data;

    if(status <= 0) {
        tunnelError(tunnel, 504,
                    internAtomError(-status, 
                                    "Host %s lookup failed",
                                    atomString(tunnel->hostname)));
        return 1;
    }

    if(request->addr->string[0] == DNS_CNAME) {
        if(request->count > 10)
            tunnelError(tunnel, 504, internAtom("CNAME loop"));
        do_gethostbyname(request->addr->string + 1, request->count + 1,
                         tunnelDnsHandler, tunnel);
        return 1;
    }

    do_connect(retainAtom(request->addr), 0,
               parentHost ? parentPort : tunnel->port,
               tunnelConnectionHandler, tunnel);
    return 1;
}