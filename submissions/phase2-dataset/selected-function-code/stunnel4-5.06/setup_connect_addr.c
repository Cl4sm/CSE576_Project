#ifdef SO_ORIGINAL_DST
    socklen_t addrlen=sizeof(SOCKADDR_UNION);
#endif /* SO_ORIGINAL_DST */

    /* process "redirect" first */
    if(c->redirect==REDIRECT_ON) {
        s_log(LOG_NOTICE, "Redirecting connection");
        if(c->connect_addr.addr) /* allocated in protocol negotiations */
            str_free(c->connect_addr.addr);
        addrlist_dup(&c->connect_addr, &c->opt->redirect_addr);
        return;
    }

    /* check if the address was already set in protocol negotiations */
    /* used by the following protocols: CONNECT */
    if(c->connect_addr.num)
        return;

    /* transparent destination */
#ifdef SO_ORIGINAL_DST
    if(c->opt->option.transparent_dst) {
        c->connect_addr.num=1;
        c->connect_addr.addr=str_alloc(sizeof(SOCKADDR_UNION));
        if(getsockopt(c->local_rfd.fd, SOL_IP, SO_ORIGINAL_DST,
                c->connect_addr.addr, &addrlen)) {
            sockerror("setsockopt SO_ORIGINAL_DST");
            longjmp(c->err, 1);
        }
        c->connect_addr.rr_val=0;
        c->connect_addr.rr_ptr=&c->connect_addr.rr_val;
        return;
    }
#endif /* SO_ORIGINAL_DST */

    /* default "connect" target */
    addrlist_dup(&c->connect_addr, &c->opt->connect_addr);
}
