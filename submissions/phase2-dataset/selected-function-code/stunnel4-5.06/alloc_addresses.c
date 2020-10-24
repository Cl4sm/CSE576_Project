        u_short port, struct addrinfo **head, struct addrinfo **tail) {
    int i;
    struct addrinfo *ai;

    /* copy addresses */
    for(i=0; h->h_addr_list[i]; i++) {
        ai=str_alloc(sizeof(struct addrinfo));
        if(hints)
            memcpy(ai, hints, sizeof(struct addrinfo));
        ai->ai_next=NULL; /* just in case */
        if(*tail) { /* list not empty: add a node */
            (*tail)->ai_next=ai;
            *tail=ai;
        } else { /* list empty: create it */
            *head=ai;
            *tail=ai;
        }
        ai->ai_family=h->h_addrtype;
#if defined(USE_IPv6)
        if(h->h_addrtype==AF_INET6) {
            ai->ai_addrlen=sizeof(struct sockaddr_in6);
            ai->ai_addr=str_alloc(ai->ai_addrlen);
            memcpy(&((struct sockaddr_in6 *)ai->ai_addr)->sin6_addr,
                h->h_addr_list[i], h->h_length);
        } else
#endif
        {
            ai->ai_addrlen=sizeof(struct sockaddr_in);
            ai->ai_addr=str_alloc(ai->ai_addrlen);
            memcpy(&((struct sockaddr_in *)ai->ai_addr)->sin_addr,
                h->h_addr_list[i], h->h_length);
        }
        ai->ai_addr->sa_family=h->h_addrtype;
        /* offsets of sin_port and sin6_port should be the same */
        ((struct sockaddr_in *)ai->ai_addr)->sin_port=port;
    }
    return 0; /* success */
}
