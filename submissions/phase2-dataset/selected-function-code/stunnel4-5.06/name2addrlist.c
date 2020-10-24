unsigned name2addrlist(SOCKADDR_LIST *addr_list, char *name, char *default_host) {
    char *tmp, *host_name, *port_name;
    unsigned retval;

    addr_list->rr_val=0; /* reset round-robin counter */
    /* allow structures created with sockaddr_dup() to modify
     * the original rr_val rather than its local copy */
    addr_list->rr_ptr=&addr_list->rr_val;

    /* first check if this is a UNIX socket */
#ifdef HAVE_STRUCT_SOCKADDR_UN
    if(*name=='/') {
        if(offsetof(struct sockaddr_un, sun_path)+strlen(name)+1
                > sizeof(struct sockaddr_un)) {
            s_log(LOG_ERR, "Unix socket path is too long");
            return 0; /* no results */
        }
        addr_list->addr=str_realloc(addr_list->addr,
            (addr_list->num+1)*sizeof(SOCKADDR_UNION));
        addr_list->addr[addr_list->num].un.sun_family=AF_UNIX;
        strcpy(addr_list->addr[addr_list->num].un.sun_path, name);
        return ++(addr_list->num); /* ok - return the number of addresses */
    }
#endif

    /* set host_name and port_name */
    tmp=str_dup(name);
    port_name=strrchr(tmp, ':');
    if(port_name) {
        host_name=tmp;
        *port_name++='\0';
    } else { /* no ':' - use default host IP */
        host_name=default_host;
        port_name=tmp;
    }

    /* fill addr_list structure */
    retval=hostport2addrlist(addr_list, host_name, port_name);
    str_free(tmp);
    return retval;
}
