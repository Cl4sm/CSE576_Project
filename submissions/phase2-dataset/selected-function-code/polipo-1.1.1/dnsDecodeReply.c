static int
dnsDecodeReply(char *buf, int offset, int n, int *id_return,
               AtomPtr *name_return, AtomPtr *value_return,
               int *af_return, unsigned *ttl_return)
{
    int i = offset, j, m;
    int id = -1, b23, qdcount, ancount, nscount, arcount, rdlength;
    int class, type;
    unsigned int ttl;
    char b[2048];
    int af = -1;
    AtomPtr name = NULL, value;
    char addresses[1024];
    int addr_index = 0;
    int error = EDNS_NO_ADDRESS;
    unsigned final_ttl = 7 * 24 * 3600;
    int dnserror;

    if(n - i < 12) {
        error = EDNS_INVALID;
        goto fail;
    }

    DO_NTOHS(id, &buf[i]); i += 2;
    DO_NTOHS(b23, &buf[i]); i += 2;
    DO_NTOHS(qdcount, &buf[i]); i += 2;
    DO_NTOHS(ancount, &buf[i]); i += 2;
    DO_NTOHS(nscount, &buf[i]); i += 2;
    DO_NTOHS(arcount, &buf[i]); i += 2;

    do_log(D_DNS, 
           "DNS id %d, b23 0x%x, qdcount %d, ancount %d, "
           "nscount %d, arcount %d\n",
           id, b23, qdcount, ancount, nscount, arcount);

    if((b23 & (0xF870)) != 0x8000) {
        do_log(L_ERROR, "Incorrect DNS reply (b23 = 0x%x).\n", b23);
        error = EDNS_INVALID;
        goto fail;
    }

    dnserror = b23 & 0xF;

    if(b23 & 0x200) {
        do_log(L_WARN, "Truncated DNS reply (b23 = 0x%x).\n", b23);
    }

    if(dnserror || qdcount != 1) {
        if(!dnserror)
            do_log(L_ERROR, 
                   "Unexpected number %d of DNS questions.\n", qdcount);
        if(dnserror == 1)
            error = EDNS_FORMAT;
        else if(dnserror == 2)
            error = EDNS_NO_RECOVERY;
        else if(dnserror == 3)
            error = EDNS_HOST_NOT_FOUND;
        else if(dnserror == 4 || dnserror == 5)
            error = EDNS_REFUSED;
        else if(dnserror == 0)
            error = EDNS_INVALID;
        else
            error = EUNKNOWN;
        goto fail;
    }

    /* We do this early, so that we can return the address family to
       the caller in case of error. */
    i = labelsToString(buf, i, n, b, 2048, &m);
    if(i < 0) {
        error = EDNS_FORMAT;
        goto fail;
    }
    DO_NTOHS(type, &buf[i]); i += 2;
    DO_NTOHS(class, &buf[i]); i += 2;

    if(type == 1)
        af = 4;
    else if(type == 28)
        af = 6;
    else {
        error = EDNS_FORMAT;
        goto fail;
    }

    do_log(D_DNS, "DNS q: ");
    do_log_n(D_DNS, b, m);
    do_log(D_DNS, " (%d, %d)\n", type, class);
    name = internAtomLowerN(b, m);
    if(name == NULL) {
        error = ENOMEM;
        goto fail;
    }

    if(class != 1) {
        error = EDNS_FORMAT;
        goto fail;
    }

#define PARSE_ANSWER(kind, label) \
do { \
    i = labelsToString(buf, i, 1024, b, 2048, &m); \
    if(i < 0) goto label; \
    DO_NTOHS(type, &buf[i]); i += 2; if(i > 1024) goto label; \
    DO_NTOHS(class, &buf[i]); i += 2; if(i > 1024) goto label; \
    DO_NTOHL(ttl, &buf[i]); i += 4; if(i > 1024) goto label; \
    DO_NTOHS(rdlength, &buf[i]); i += 2; if(i > 1024) goto label; \
    do_log(D_DNS, "DNS " kind ": "); \
    do_log_n(D_DNS, b, m); \
    do_log(D_DNS, " (%d, %d): %d bytes, ttl %u\n", \
           type, class, rdlength, ttl); \
   } while(0)


    for(j = 0; j < ancount; j++) {
        PARSE_ANSWER("an", fail);
        if(strcasecmp_n(name->string, b, m) == 0) {
            if(class != 1) {
                do_log(D_DNS, "DNS: %s: unknown class %d.\n", 
                       name->string, class);
                error = EDNS_UNSUPPORTED;
                goto cont;
            }
            if(type == 1 || type == 28) {
                if((type == 1 && rdlength != 4) ||
                   (type == 28 && rdlength != 16)) {
                    do_log(L_ERROR, 
                           "DNS: %s: unexpected length %d of %s record.\n",
                           scrub(name->string),
                           rdlength, type == 1 ? "A" : "AAAA");
                    error = EDNS_INVALID;
                    if(rdlength <= 0 || rdlength >= 32)
                        goto fail;
                    goto cont;
                }
                if(af == 0) {
                    do_log(L_WARN, "DNS: %s: host has both A and CNAME -- "
                           "ignoring CNAME.\n", scrub(name->string));
                    addr_index = 0;
                    af = -1;
                }
                if(type == 1) {
                    if(af < 0)
                        af = 4;
                    else if(af == 6) {
                        do_log(L_WARN, "Unexpected AAAA reply.\n");
                        goto cont;
                    }
                } else {
                    if(af < 0)
                        af = 6;
                    else if(af == 4) {
                        do_log(L_WARN, "Unexpected A reply.\n");
                        goto cont;
                    }
                }

                if(addr_index == 0) {
                    addresses[0] = DNS_A;
                    addr_index++;
                } else {
                    if(addr_index > 1000) {
                        error = EDNS_INVALID;
                        goto fail;
                    }
                }
                assert(addresses[0] == DNS_A);
                if(final_ttl > ttl)
                    final_ttl = ttl;
                memset(&addresses[addr_index], 0, sizeof(HostAddressRec));
                if(type == 1) {
                    addresses[addr_index] = 4;
                    memcpy(addresses + addr_index + 1, buf + i, 4);
                } else {
                    addresses[addr_index] = 6;
                    memcpy(addresses + addr_index + 1, buf + i, 16);
                }
                addr_index += sizeof(HostAddressRec);
            } else if(type == 5) {
                int j, k;
                if(af != 0 && addr_index > 0) {
                    do_log(L_WARN, "DNS: host has both CNAME and A -- "
                           "ignoring CNAME.\n");
                    goto cont;
                }
                af = 0;

                if(addr_index != 0) {
                    /* Only warn if the CNAMEs are not identical */
                    char tmp[512]; int jj, kk;
                    assert(addresses[0] == DNS_CNAME);
                    jj = labelsToString(buf, i, n,
                                        tmp, 512, &kk);
                    if(jj < 0 ||
                       kk != strlen(addresses + 1) ||
                       memcmp(addresses + 1, tmp, kk) != 0) {
                        do_log(L_WARN, "DNS: "
                               "%s: host has multiple CNAMEs -- "
                               "ignoring subsequent.\n",
                               scrub(name->string));

                    }
                    goto cont;
                }
                addresses[0] = DNS_CNAME;
                addr_index++;
                j = labelsToString(buf, i, n,
                                   addresses + 1, 1020, &k);
                if(j < 0) {
                    addr_index = 0;
                    error = ENAMETOOLONG;
                    continue;
                }
                addr_index = k + 1;
            } else {
                error = EDNS_NO_ADDRESS;
                i += rdlength;
                continue;
            }

        }
    cont:
        i += rdlength;
    }

#if (LOGGING_MAX & D_DNS)
    for(j = 0; j < nscount; j++) {
        PARSE_ANSWER("ns", nofail);
        i += rdlength;
    }

    for(j = 0; j < arcount; j++) {
        PARSE_ANSWER("ar", nofail);
        i += rdlength;
    }

 nofail:
#endif

#undef PARSE_ANSWER

    do_log(D_DNS, "DNS: %d bytes\n", addr_index);
    if(af < 0)
        goto fail;

    value = internAtomN(addresses, addr_index);
    if(value == NULL) {
        error = ENOMEM;
        goto fail;
    }

    assert(af >= 0);
    *id_return = id;
    *name_return = name;
    *value_return = value;
    *af_return = af;
    *ttl_return = final_ttl;
    return 1;

 fail:
    *id_return = id;
    *name_return = name;
    *value_return = NULL;
    *af_return = af;
    return -error;
}