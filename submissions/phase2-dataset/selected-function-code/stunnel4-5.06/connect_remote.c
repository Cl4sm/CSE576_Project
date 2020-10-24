    int fd;
    unsigned ind_start, ind_try, ind_cur;

    setup_connect_addr(c);
    if(!c->connect_addr.num) {
        s_log(LOG_ERR, "No host resolved");
        longjmp(c->err, 1);
    }
    ind_start=*c->connect_addr.rr_ptr;
    /* the race condition here can be safely ignored */
    if(c->opt->failover==FAILOVER_RR)
        *c->connect_addr.rr_ptr=(ind_start+1)%c->connect_addr.num;

    /* try to connect each host from the list */
    for(ind_try=0; ind_try<c->connect_addr.num; ind_try++) {
        ind_cur=(ind_start+ind_try)%c->connect_addr.num;
        c->fd=s_socket(c->connect_addr.addr[ind_cur].sa.sa_family,
            SOCK_STREAM, 0, 1, "remote socket");
        if(c->fd<0)
            longjmp(c->err, 1);

        local_bind(c); /* explicit local bind or transparent proxy */

        if(s_connect(c, &c->connect_addr.addr[ind_cur],
                addr_len(&c->connect_addr.addr[ind_cur]))) {
            closesocket(c->fd);
            c->fd=-1;
            continue; /* next IP */
        }
        print_bound_address(c);
        fd=c->fd;
        c->fd=-1;
        return fd; /* success! */
    }
    longjmp(c->err, 1);
    return -1; /* some C compilers require a return value */
}
