NOEXPORT int hostport2addrlist(SOCKADDR_LIST *addr_list,
        char *host_name, char *port_name) {
    struct addrinfo hints, *res=NULL, *cur;
    int err, retries=0;

    memset(&hints, 0, sizeof hints);
#if defined(USE_IPv6) || defined(USE_WIN32)
    hints.ai_family=PF_UNSPEC;
#else
    hints.ai_family=PF_INET;
#endif
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_protocol=IPPROTO_TCP;
    for(;;) {
        err=getaddrinfo(host_name, port_name, &hints, &res);
        if(err && res)
            freeaddrinfo(res);
        if(err!=EAI_AGAIN || ++retries>=3)
            break;
        s_log(LOG_DEBUG, "getaddrinfo: EAI_AGAIN received: retrying");
        sleep(1);
    }
    switch(err) {
    case 0:
        break; /* success */
    case EAI_SERVICE:
        s_log(LOG_ERR, "Unknown TCP service '%s'", port_name);
        return 0; /* error */
    default:
        s_log(LOG_ERR, "Error resolving '%s': %s",
            host_name, s_gai_strerror(err));
        return 0; /* error */
    }

    /* copy the list of addresses */
    for(cur=res; cur; cur=cur->ai_next) {
        if(cur->ai_addrlen>(int)sizeof(SOCKADDR_UNION)) {
            s_log(LOG_ERR, "INTERNAL ERROR: ai_addrlen value too big");
            freeaddrinfo(res);
            return 0; /* no results */
        }
        addr_list->addr=str_realloc(addr_list->addr,
            (addr_list->num+1)*sizeof(SOCKADDR_UNION));
        memcpy(&addr_list->addr[addr_list->num], cur->ai_addr, cur->ai_addrlen);
        ++(addr_list->num);
    }
    freeaddrinfo(res);
    return addr_list->num; /* ok - return the number of addresses */
}
