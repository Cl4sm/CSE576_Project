NOEXPORT int servername_cb(SSL *ssl, int *ad, void *arg) {
    SERVICE_OPTIONS *section=(SERVICE_OPTIONS *)arg;
    const char *servername=SSL_get_servername(ssl, TLSEXT_NAMETYPE_host_name);
    SERVERNAME_LIST *list;
    CLI *c;
#ifdef USE_LIBWRAP
    char *accepted_address;
#endif /* USE_LIBWRAP */

    /* leave the alert type at SSL_AD_UNRECOGNIZED_NAME */
    (void)ad; /* skip warning about unused parameter */
    if(!section->servername_list_head) {
        s_log(LOG_DEBUG, "SNI: no virtual services defined");
        return SSL_TLSEXT_ERR_OK;
    }
    if(!servername) {
        s_log(LOG_NOTICE, "SNI: no servername received");
        return SSL_TLSEXT_ERR_NOACK;
    }
    s_log(LOG_INFO, "SNI: requested servername: %s", servername);

    for(list=section->servername_list_head; list; list=list->next)
        if(matches_wildcard((char *)servername, list->servername)) {
            s_log(LOG_DEBUG, "SNI: matched pattern: %s", list->servername);
            c=SSL_get_ex_data(ssl, cli_index);
            c->opt=list->opt;
            SSL_set_SSL_CTX(ssl, c->opt->ctx);
            SSL_set_verify(ssl, SSL_CTX_get_verify_mode(c->opt->ctx),
                SSL_CTX_get_verify_callback(c->opt->ctx));
            s_log(LOG_NOTICE, "SNI: switched to service [%s]",
                c->opt->servname);
#ifdef USE_LIBWRAP
            accepted_address=s_ntop(&c->peer_addr, c->peer_addr_len);
            libwrap_auth(c, accepted_address); /* retry on a service switch */
            str_free(accepted_address);
#endif /* USE_LIBWRAP */
            return SSL_TLSEXT_ERR_OK;
        }
    s_log(LOG_ERR, "SNI: no pattern matched servername: %s", servername);
    return SSL_TLSEXT_ERR_ALERT_FATAL;
}
