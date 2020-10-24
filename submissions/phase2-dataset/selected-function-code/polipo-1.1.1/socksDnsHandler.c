static int
socksDnsHandler(int status, GethostbynameRequestPtr grequest)
{
    SocksRequestPtr request = grequest->data;
    if(status <= 0) {
        request->handler(status, request);
        destroySocksRequest(request);
        return 1;
    }

    if(grequest->addr->string[0] == DNS_CNAME) {
        if(grequest->count > 10) {
            do_log(L_ERROR, "DNS CNAME loop.\n");
            request->handler(-EDNS_CNAME_LOOP, request);
            destroySocksRequest(request);
            return 1;
        }
        do_gethostbyname(grequest->addr->string + 1, grequest->count + 1,
                         socksDnsHandler, request);
        return 1;
    }


    socksProxyAddress = retainAtom(grequest->addr);
    socksProxyAddressIndex = 0;

    do_socks_connect_common(request);
    return 1;
}