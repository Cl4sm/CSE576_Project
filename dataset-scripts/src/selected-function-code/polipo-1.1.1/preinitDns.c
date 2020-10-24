void
preinitDns()
{
#ifdef HAVE_IPv6
    int fd;
#endif

    assert(sizeof(struct in_addr) == 4);
#ifdef HAVE_IPv6
    assert(sizeof(struct in6_addr) == 16);
#endif

#ifndef NO_STANDARD_RESOLVER
    CONFIG_VARIABLE(dnsGethostbynameTtl, CONFIG_TIME,
                    "TTL for gethostbyname addresses.");
#endif

#ifdef HAVE_IPv6
    fd = socket(PF_INET6, SOCK_STREAM, 0);
    if(fd < 0) {
        if(errno == EPROTONOSUPPORT || errno == EAFNOSUPPORT) {
            dnsQueryIPv6 = 0;
        } else {
            do_log_error(L_WARN, errno, "DNS: couldn't create socket");
        }
    } else {
        close(fd);
    }
#endif

#ifndef NO_FANCY_RESOLVER
#ifndef WIN32
    parseResolvConf("/etc/resolv.conf");
#endif
    if(dnsNameServer == NULL || dnsNameServer->string[0] == '\0')
        dnsNameServer = internAtom("127.0.0.1");
    CONFIG_VARIABLE(dnsMaxTimeout, CONFIG_TIME,
                    "Max timeout for DNS queries.");
    CONFIG_VARIABLE(dnsNegativeTtl, CONFIG_TIME,
                    "TTL for negative DNS replies with no TTL.");
    CONFIG_VARIABLE(dnsNameServer, CONFIG_ATOM_LOWER,
                    "The name server to use.");
#ifndef NO_STANDARD_RESOLVER
    CONFIG_VARIABLE(dnsUseGethostbyname, CONFIG_TETRASTATE,
                    "Use the system resolver.");
#endif
#endif

#ifdef HAVE_IPv6
    CONFIG_VARIABLE(dnsQueryIPv6, CONFIG_TETRASTATE,
                    "Query for IPv6 addresses.");
#endif
}