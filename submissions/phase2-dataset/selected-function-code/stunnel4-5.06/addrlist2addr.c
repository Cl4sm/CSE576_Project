    unsigned i;

    for(i=0; i<addr_list->num; ++i) { /* find the first IPv4 address */
        if(addr_list->addr[i].in.sin_family==AF_INET) {
            memcpy(addr, &addr_list->addr[i], sizeof(SOCKADDR_UNION));
            return;
        }
    }
#ifdef USE_IPv6
    for(i=0; i<addr_list->num; ++i) { /* find the first IPv6 address */
        if(addr_list->addr[i].in.sin_family==AF_INET6) {
            memcpy(addr, &addr_list->addr[i], sizeof(SOCKADDR_UNION));
            return;
        }
    }
#endif
    /* copy the first address resolved (curently AF_UNIX) */
    memcpy(addr, &addr_list->addr[0], sizeof(SOCKADDR_UNION));
}
