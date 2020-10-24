void
initDns()
{
#ifndef NO_FANCY_RESOLVER
    int rc;
    struct timeval t;
    struct sockaddr_in *sin = (struct sockaddr_in*)&nameserverAddress;
#ifdef HAVE_IPv6
    struct sockaddr_in6 *sin6 = (struct sockaddr_in6*)&nameserverAddress;
#endif

    atomLocalhost = internAtom("localhost");
    atomLocalhostDot = internAtom("localhost.");
    inFlightDnsQueries = NULL;
    inFlightDnsQueriesLast = NULL;

    gettimeofday(&t, NULL);
    idSeed = t.tv_usec & 0xFFFF;
    sin->sin_family = AF_INET;
    sin->sin_port = htons(53);
    rc = inet_aton(dnsNameServer->string, &sin->sin_addr);
#ifdef HAVE_IPv6
    if(rc != 1) {
        sin6->sin6_family = AF_INET6;
        sin6->sin6_port = htons(53);
        rc = inet_pton(AF_INET6, dnsNameServer->string, &sin6->sin6_addr);
    }
#endif
    if(rc != 1) {
        do_log(L_ERROR, "DNS: couldn't parse name server %s.\n",
               dnsNameServer->string);
        exit(1);
    }
#endif
}