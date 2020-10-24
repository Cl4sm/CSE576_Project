void
initHttp()
{
    char *buf = NULL;
    int namelen;
    int n;
    struct hostent *host;

    initHttpParser();

    atom100Continue = internAtom("100-continue");

    if(clientTimeout <= serverTimeout) {
        clientTimeout = serverTimeout + 1;
        do_log(L_WARN, "Value of clientTimeout too small -- setting to %d.\n",
               clientTimeout);
    }

    if(displayName == NULL)
        displayName = internAtom("Polipo");

    if(authCredentials != NULL && authRealm == NULL)
        authRealm = internAtom("Polipo");

    if(allowedClients) {
        allowedNets = parseNetAddress(allowedClients);
        if(allowedNets == NULL)
            exit(1);
    }

    if(allowedPorts == NULL) {
        allowedPorts = makeIntList(0);
        if(allowedPorts == NULL) {
            do_log(L_ERROR, "Couldn't allocate allowedPorts.\n");
            exit(1);
        }
        intListCons(80, 100, allowedPorts);
        intListCons(1024, 0xFFFF, allowedPorts);
    }

    if(tunnelAllowedPorts == NULL) {
        tunnelAllowedPorts = makeIntList(0);
        if(tunnelAllowedPorts == NULL) {
            do_log(L_ERROR, "Couldn't allocate tunnelAllowedPorts.\n");
            exit(1);
        }
        intListCons(22, 22, tunnelAllowedPorts);   /* ssh */
        intListCons(80, 80, tunnelAllowedPorts);   /* HTTP */
        intListCons(109, 110, tunnelAllowedPorts); /* POP 2 and 3*/
        intListCons(143, 143, tunnelAllowedPorts); /* IMAP 2/4 */
        intListCons(443, 443, tunnelAllowedPorts); /* HTTP/SSL */
        intListCons(873, 873, tunnelAllowedPorts); /* rsync */
        intListCons(993, 993, tunnelAllowedPorts); /* IMAP/SSL */
        intListCons(995, 995, tunnelAllowedPorts); /* POP/SSL */
        intListCons(2401, 2401, tunnelAllowedPorts); /* CVS */
        intListCons(5222, 5223, tunnelAllowedPorts); /* Jabber */
        intListCons(9418, 9418, tunnelAllowedPorts); /* Git */
    }

    if(proxyName)
        return;

    buf = get_chunk();
    if(buf == NULL) {
        do_log(L_ERROR, "Couldn't allocate chunk for host name.\n");
        goto fail;
    }

    n = gethostname(buf, CHUNK_SIZE);
    if(n != 0) {
        do_log_error(L_WARN, errno, "Gethostname");
        strcpy(buf, "polipo");
        goto success;
    }
    /* gethostname doesn't necessarily NUL-terminate on overflow */
    buf[CHUNK_SIZE - 1] = '\0';

    if(strcmp(buf, "(none)") == 0 ||
       strcmp(buf, "localhost") == 0 ||
       strcmp(buf, "localhost.localdomain") == 0) {
        do_log(L_WARN, "Couldn't determine host name -- using ``polipo''.\n");
        strcpy(buf, "polipo");
        goto success;
    }

    if(strchr(buf, '.') != NULL)
        goto success;

    host = gethostbyname(buf);
    if(host == NULL) {
        goto success;
    }

    if(host->h_addrtype != AF_INET)
        goto success;

    host = gethostbyaddr(host->h_addr_list[0], host->h_length,  AF_INET);

    if(!host || !host->h_name || strcmp(host->h_name, "localhost") == 0 ||
       strcmp(host->h_name, "localhost.localdomain") == 0)
        goto success;

    namelen = strlen(host->h_name);
    if(namelen >= CHUNK_SIZE) {
        do_log(L_ERROR, "Host name too long.\n");
        goto success;
    }

    memcpy(buf, host->h_name, namelen + 1);

 success:
    proxyName = internAtom(buf);
    if(proxyName == NULL) {
        do_log(L_ERROR, "Couldn't allocate proxy name.\n");
        goto fail;
    }
    dispose_chunk(buf);
    return;

 fail:
    if(buf)
        dispose_chunk(buf);
    exit(1);
    return;
}