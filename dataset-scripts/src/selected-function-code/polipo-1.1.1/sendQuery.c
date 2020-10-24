static int
sendQuery(DnsQueryPtr query)
{
    char buf[512];
    int buflen;
    int rc;
    int af[2];
    int i;

    if(dnsSocket < 0)
        return -1;

    if(dnsQueryIPv6 <= 0) {
        af[0] = 4; af[1] = 0;
    } else if(dnsQueryIPv6 <= 2) {
        af[0] = 4; af[1] = 6;
    } else {
        af[0] = 6; af[1] = 0;
    }

    for(i = 0; i < 2; i++) {
        if(af[i] == 0)
            continue;
        if(af[i] == 4 && query->inet4)
            continue;
        else if(af[i] == 6 && query->inet6)
            continue;

        buflen = dnsBuildQuery(query->id, buf, 0, 512, query->name, af[i]);
        if(buflen <= 0) {
            do_log(L_ERROR, "Couldn't build DNS query.\n");
            return buflen;
        }

        rc = send(dnsSocket, buf, buflen, 0);
        if(rc < buflen) {
            if(rc >= 0) {
                do_log(L_ERROR, "Couldn't send DNS query: partial send.\n");
                return -EAGAIN;
            } else {
                do_log_error(L_ERROR, errno, "Couldn't send DNS query");
                return -errno;
            }
        }
    }
    return 1;
}