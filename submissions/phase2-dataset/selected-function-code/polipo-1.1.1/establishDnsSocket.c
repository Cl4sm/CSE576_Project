static int
establishDnsSocket()
{
    int rc;
#ifdef HAVE_IPv6
    int inet6 = (nameserverAddress.sa_family == AF_INET6);
    int pf = inet6 ? PF_INET6 : PF_INET;
    int sa_size = 
        inet6 ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in);
#else
    int pf = PF_INET;
    int sa_size = sizeof(struct sockaddr_in);
#endif

    if(dnsSocket < 0) {
        assert(!dnsSocketHandler);
        dnsSocket = socket(pf, SOCK_DGRAM, 0);
        if(dnsSocket < 0) {
            do_log_error(L_ERROR, errno, "Couldn't create DNS socket");
            return -errno;
        }

        rc = connect(dnsSocket, &nameserverAddress, sa_size);
        if(rc < 0) {
            CLOSE(dnsSocket);
            dnsSocket = -1;
            do_log_error(L_ERROR, errno, "Couldn't create DNS \"connection\"");
            return -errno;
        }
    }

    if(!dnsSocketHandler) {
        dnsSocketHandler = 
            registerFdEvent(dnsSocket, POLLIN, dnsReplyHandler, 0, NULL);
        if(dnsSocketHandler == NULL) {
            do_log(L_ERROR, "Couldn't register DNS socket handler.\n");
            CLOSE(dnsSocket);
            dnsSocket = -1;
            return -ENOMEM;
        }
    }

    return 1;
}