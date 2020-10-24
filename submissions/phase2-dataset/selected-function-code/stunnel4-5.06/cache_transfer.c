        const unsigned int timeout,
        const unsigned char *key, const unsigned int key_len,
        const unsigned char *val, const unsigned int val_len,
        unsigned char **ret, unsigned int *ret_len) {
    char session_id_txt[2*SSL_MAX_SSL_SESSION_ID_LENGTH+1];
    const char hex[16]="0123456789ABCDEF";
    const char *type_description[]={"new", "get", "remove"};
    unsigned int i;
    int s, len;
    struct timeval t;
    CACHE_PACKET *packet;
    SERVICE_OPTIONS *section;

    if(ret) /* set error as the default result if required */
        *ret=NULL;

    /* log the request information */
    for(i=0; i<key_len && i<SSL_MAX_SSL_SESSION_ID_LENGTH; ++i) {
        session_id_txt[2*i]=hex[key[i]>>4];
        session_id_txt[2*i+1]=hex[key[i]&0x0f];
    }
    session_id_txt[2*i]='\0';
    s_log(LOG_INFO,
        "cache_transfer: request=%s, timeout=%u, id=%s, length=%d",
        type_description[type], timeout, session_id_txt, val_len);

    /* allocate UDP packet buffer */
    if(key_len>SSL_MAX_SSL_SESSION_ID_LENGTH) {
        s_log(LOG_ERR, "cache_transfer: session id too big (%d bytes)",
            key_len);
        return;
    }
    if(val_len>MAX_VAL_LEN) {
        s_log(LOG_ERR, "cache_transfer: encoded session too big (%d bytes)",
            key_len);
        return;
    }
    packet=str_alloc(sizeof(CACHE_PACKET));

    /* setup packet */
    packet->version=1;
    packet->type=type;
    packet->timeout=htons((u_short)(timeout<64800?timeout:64800));/* 18 hours */
    memcpy(packet->key, key, key_len);
    memcpy(packet->val, val, val_len);

    /* create the socket */
    s=s_socket(AF_INET, SOCK_DGRAM, 0, 0, "cache_transfer: socket");
    if(s<0) {
        str_free(packet);
        return;
    }

    /* retrieve pointer to the section structure of this ctx */
    section=SSL_CTX_get_ex_data(ctx, opt_index);
    if(sendto(s, (void *)packet, sizeof(CACHE_PACKET)-MAX_VAL_LEN+val_len, 0,
            &section->sessiond_addr.sa, addr_len(&section->sessiond_addr))<0) {
        sockerror("cache_transfer: sendto");
        closesocket(s);
        str_free(packet);
        return;
    }

    if(!ret || !ret_len) { /* no response is required */
        closesocket(s);
        str_free(packet);
        return;
    }

    /* set recvfrom timeout to 200ms */
    t.tv_sec=0;
    t.tv_usec=200;
    if(setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (void *)&t, sizeof t)<0) {
        sockerror("cache_transfer: setsockopt SO_RCVTIMEO");
        closesocket(s);
        str_free(packet);
        return;
    }

    /* retrieve response */
    len=recv(s, (void *)packet, sizeof(CACHE_PACKET), 0);
    closesocket(s);
    if(len<0) {
        if(get_last_socket_error()==S_EWOULDBLOCK ||
                get_last_socket_error()==S_EAGAIN)
            s_log(LOG_INFO, "cache_transfer: recv timeout");
        else
            sockerror("cache_transfer: recv");
        str_free(packet);
        return;
    }

    /* parse results */
    if(len<(int)sizeof(CACHE_PACKET)-MAX_VAL_LEN || /* too short */
            packet->version!=1 || /* wrong version */
            safe_memcmp(packet->key, key, key_len)) { /* wrong session id */
        s_log(LOG_DEBUG, "cache_transfer: malformed packet received");
        str_free(packet);
        return;
    }
    if(packet->type!=CACHE_RESP_OK) {
        s_log(LOG_INFO, "cache_transfer: session not found");
        str_free(packet);
        return;
    }
    *ret_len=len-(sizeof(CACHE_PACKET)-MAX_VAL_LEN);
    *ret=str_alloc(*ret_len);
    s_log(LOG_INFO, "cache_transfer: session found");
    memcpy(*ret, packet->val, *ret_len);
    str_free(packet);
}
